
CFLAGS += -g -Wall
CFLAGS += -std=c++11
CFLAGS += -I.

LIBS += -levent -lpthread

OUT_dir := "./out/"

Webserver_exe = $(OUT_dir)executor
MME_SRC = main.cpp deferredtasksexecutor.cpp deferredtask.cpp deferredtaskpool.cpp task.cpp

all: $(Webserver_exe)

prepare_dir:
	mkdir -p $(OUT_dir)

$(Webserver_exe): $(MME_SRC) | prepare_dir
	$(CXX) $(CFLAGS) $(MME_SRC) -o $@  $(LIBS)


clean:
	rm -rf $(OUT_dir)