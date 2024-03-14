package main

import (
	"flag"
	"log"
	"net"
	"os"
	"sync"
	"time"
)

var (
	addr   string
	remo   string
	logger *log.Logger
)

func init() {
	src := flag.String("l", "", "Listen address and port, e.g: 0.0.0.0:8123")
	dst := flag.String("r", "", "Remote address and port, e.g: 192.168.0.1:1234")
	flag.Parse()
	if *src == "" || *dst == "" {
		flag.Usage()
		os.Exit(-1)
	}
	addr, remo = *src, *dst
	logger = log.New(os.Stdout, "", log.LstdFlags)
}

func main() {
	ser, err := net.Listen("tcp", addr)
	if err != nil {
		logger.Println(err.Error())
		return
	}
	logger.Println("start proxy:", addr)
	for {
		conn, err := ser.Accept()
		if err != nil {
			logger.Println("accept error:", err.Error())
			break
		}
		go handle(conn)
	}
}

func handle(conn net.Conn) {
	logger.Printf("  <%s> established\n", conn.RemoteAddr().String())
	defer conn.Close()
	cli, err := net.DialTimeout("tcp", remo, time.Second*5)
	if err != nil {
		logger.Println(err.Error())
		return
	}
	wg := sync.WaitGroup{}
	wg.Add(2)
	go pipe(conn, cli, &wg)
	go pipe(cli, conn, &wg)
	wg.Wait()
	logger.Printf("  <%s> closed\n", conn.RemoteAddr().String())
}

func pipe(r, w net.Conn, wg *sync.WaitGroup) {
	defer wg.Done()
	buf := make([]byte, 1024)
	for {
		if n, e := r.Read(buf); e != nil || n == 0 {
			w.Close()
			break
		} else {
			w.Write(buf[:n])
		}
	}
}
