package main

import (
	"bytes"
	"encoding/json"
	"io/ioutil"
	"log"
	"net/http"
	"testing"
	"time"
)

// go test -run=^TestHttpClient$ -v http_test.go
func TestHttpClient(t *testing.T) {
	const url = "http://127.0.0.1:8899?page=1&count=20&user=false"
	client := http.Client{Timeout: time.Second * 2}

	buf, _ := json.Marshal(`{"id": 123, "name": "tom"}`)
	buffer := bytes.NewBuffer(buf)

	request, err := http.NewRequest(http.MethodPost, url, buffer)
	if err != nil {
		t.Fatal(err.Error())
	}
	request.Header.Add("User-Agent", "test-user-agent")
	request.Header.Add("Content-Type", "application/json")

	resp, err := client.Do(request)
	if err != nil {
		t.Fatal(err.Error())
	}

	t.Logf("status: %v", resp.StatusCode)
}

// go test -run=^TestHttpServer$ -v http_test.go
func TestHttpServer(t *testing.T) {
	http.HandleFunc("/", func(w http.ResponseWriter, req *http.Request) {
		// response
		defer w.WriteHeader(http.StatusOK)

		log.Printf("Path | %v", req.URL.Path)
		log.Printf("Remote | %v", req.RemoteAddr)
		// Get URL query
		for k, v := range req.URL.Query() {
			log.Printf("Query | %v: %v\n", k, v[0])
		}
		// Get Headers
		for k, v := range req.Header {
			log.Printf("Header | %v: %v\n", k, v[0])
		}
		// Get Body
		if req.Method == http.MethodPost {
			if data, err := ioutil.ReadAll(req.Body); err == nil {
				log.Printf("Body | %s\n", data)
			}
		}
	})

	const addr = "127.0.0.1:8899"

	t.Logf("listen: %v", addr)
	if err := http.ListenAndServe(addr, nil); err != nil {
		t.Fatal(err.Error())
	}
}
