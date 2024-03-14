package main

import (
	"bytes"
	"net"
	"syscall"
	"testing"
	"time"
)

const (
	multicastGroup = "224.0.0.69"
	serverPort     = 8899
	bufsize        = 32
)

var multicastGroupIP net.IP

func init() {
	multicastGroupIP = net.ParseIP(multicastGroup).To4()
}

// go test -run=^TestMulticastClient$ -v multicast_udp_test.go
func TestMulticastClient(t *testing.T) {
	// ListenUDP function return an unconnected UDP socket. (Dial function return a connected socket)
	conn, err := net.ListenUDP("udp", &net.UDPAddr{IP: net.IPv4zero, Port: 0})
	if err != nil {
		t.Fatal(err.Error())
	}
	if _, err := conn.WriteToUDP([]byte("hello"), &net.UDPAddr{IP: multicastGroupIP, Port: serverPort}); err != nil {
		t.Fatal(err.Error())
	}
	buf := make([]byte, bufsize)
	conn.SetReadDeadline(time.Now().Add(time.Second * 2))
	rn, addr, err := conn.ReadFrom(buf)
	if err != nil {
		t.Fatal(err.Error())
	}
	t.Logf("data: %s, from: %v", buf[:rn], addr.String())
}

// go test -run=^TestMulticastServer$ -v multicast_udp_test.go
func TestMulticastServer(t *testing.T) {
	ifi, _ := net.InterfaceByName("en0")
	// listen UDP port and add membership
	conn, err := net.ListenMulticastUDP("udp", ifi, &net.UDPAddr{IP: multicastGroupIP, Port: serverPort})
	if err != nil {
		t.Fatal(err.Error())
	}
	buf := make([]byte, bufsize)
	if rn, addr, err := conn.ReadFromUDP(buf); err == nil {
		t.Logf("data: %s, from: %s", buf[:rn], addr.String())
		if wn, err := conn.WriteToUDP(bytes.ToUpper(buf[:rn]), addr); err != nil {
			t.Fatal(err.Error())
		} else {
			t.Log(wn)
		}
	}
}

// go test -run=^TestMulticastServerSyscall$ -v multicast_udp_test.go
func TestMulticastServerSyscall(t *testing.T) {
	fd, err := syscall.Socket(syscall.AF_INET, syscall.SOCK_DGRAM, syscall.IPPROTO_UDP)
	if err != nil {
		t.Fatal(err.Error())
	}

	// bind address and port
	var bindAddr [4]byte
	copy(bindAddr[:], net.IPv4zero)
	if err := syscall.Bind(fd, &syscall.SockaddrInet4{Addr: bindAddr, Port: serverPort}); err != nil {
		t.Fatal(err.Error())
	}

	if err := syscall.SetsockoptInt(fd, syscall.SOL_SOCKET, syscall.SO_REUSEADDR, 1); err != nil {
		t.Fatal(err.Error())
	}

	// join multicast group
	mreq := &syscall.IPMreq{}
	copy(mreq.Multiaddr[:], multicastGroupIP[:])
	if err := syscall.SetsockoptIPMreq(fd, syscall.IPPROTO_IP, syscall.IP_ADD_MEMBERSHIP, mreq); err != nil {
		t.Fatal(err.Error())
	}

	buf := make([]byte, bufsize)
	// message handler
	if rn, addr, err := syscall.Recvfrom(fd, buf, 0); err == nil {
		ip := net.IP(addr.(*syscall.SockaddrInet4).Addr[:])
		t.Logf("%v:%v, %s", ip.String(), addr.(*syscall.SockaddrInet4).Port, buf[:rn])
		if err := syscall.Sendto(fd, bytes.ToUpper(buf[:rn]), 0, addr); err != nil {
			t.Fatal(err.Error())
		}
	} else {
		t.Fatal(err.Error())
	}
}
