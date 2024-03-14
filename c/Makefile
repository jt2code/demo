SHELL := /bin/bash
TARGET_DIR := exec
SOURCR_DIR := src
OBJ_DIR := obj
LIB_DIR := lib
UTILS_DIR := utils

CC := gcc -Wall
CCO := $(CC) -o

target = $(TARGET_DIR)/$@
single_source = src/$@.c

# -----------------------------------------------------------------------------
.PHONY: clean
clean:
	@rm -f $(OBJ_DIR)/*
	@rm -f $(TARGET_DIR)/*
	@rm -f $(LIB_DIR)/*

# -----------------------------------------------------------------------------
$(OBJ_DIR)/%.o: $(UTILS_DIR)/%.c
	$(CCO) $@ -c $^
$(LIB_DIR)/libutils.a: $(addsuffix .o, $(foreach n, $(wildcard $(UTILS_DIR)/*.c), \
	$(OBJ_DIR)/$(basename $(notdir $(n)))))
	@ar -rs $@ $^

# -----------------------------------------------------------------------------
all_pthread_demo := $(foreach n, $(wildcard $(SOURCR_DIR)/pthread*.c), \
	$(basename $(notdir $(n))))
all_pthread_demo: $(all_pthread_demo)

$(all_pthread_demo):
	$(CCO) $(target) $(single_source) -pthread

# -----------------------------------------------------------------------------
popen_demo:
	$(CCO) $(target) $(single_source)

# -----------------------------------------------------------------------------
fork_demo:
	$(CCO) $(target) $(single_source)

# -----------------------------------------------------------------------------
exec_and_pipe_demo:
	$(CCO) $(target) $(single_source)

# -----------------------------------------------------------------------------
shm_open_demo:
	$(CCO) $(target) $(single_source) -lrt

# -----------------------------------------------------------------------------
shmget_demo:
	$(CCO) $(target) $(single_source)

# -----------------------------------------------------------------------------
msgqueue_demo:
	$(CCO) $(target) $(single_source)

# -----------------------------------------------------------------------------
mmap_demo:
	$(CCO) $(target) $(single_source)

# -----------------------------------------------------------------------------
epoll_demo: $(LIB_DIR)/libutils.a
	$(CCO) $(target) $(single_source) -L$(LIB_DIR) -lutils

# -----------------------------------------------------------------------------
stdarg_demo:
	$(CCO) $(target) $(single_source)

# -----------------------------------------------------------------------------
multicast_udp_demo: $(LIB_DIR)/libutils.a
	$(CCO) $(target) $(single_source) -L$(LIB_DIR) -lutils

# -----------------------------------------------------------------------------
timer_demo:
	$(CCO) $(target) $(single_source) -lrt

# -----------------------------------------------------------------------------
mq_demo:
	$(CCO) $(target) $(single_source) -lrt

# -----------------------------------------------------------------------------
signal_demo:
	$(CCO) $(target) $(single_source) -pthread

# -----------------------------------------------------------------------------
sched_demo:
	$(CCO) $(target) $(single_source)
