SHELL := /bin/bash
TARGET_DIR := bin
CMD_MAIN_SOURCE = cmd/$@/main.go

.PHONY: target_dir
target_dir:
	@if [[ ! -d $(TARGET_DIR) ]];then\
		mkdir $(TARGET_DIR);\
	fi

cmd_main_procs = tcp_proxy

.PHONY: $(cmd_main_procs)
$(cmd_main_procs): target_dir
	go build -o $(TARGET_DIR)/$@ $(CMD_MAIN_SOURCE)

.PHONY: clean
clean:
	@rm -rf $(TARGET_DIR)/*
