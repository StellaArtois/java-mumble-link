UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)

all: natives/osx/libJMumbleLibrary64.jnilib natives/osx/libJMumbleLibrary.jnilib

build/JMumbleLibrary.o: src/JMumbleLibrary.cpp
	-@mkdir $(dir $@) 
	g++ \
	-fno-rtti \
	-m32 \
	"-I/System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers" \
	"-I/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers" \
	-c $< \
	-o $@

build/JMumbleLibrary64.o: src/JMumbleLibrary.cpp
	-@mkdir $(dir $@) 
	g++ \
	-fno-rtti \
	"-I/System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers" \
	"-I/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers" \
	-c $< \
	-o $@

natives/osx/libJMumbleLibrary.jnilib: build/JMumbleLibrary.o
	-@mkdir $(dir $@) 
	g++ \
		-dynamiclib \
		-fno-rtti \
		-m32 \
		-o $@ \
		$< 

natives/osx/libJMumbleLibrary64.jnilib: build/JMumbleLibrary64.o
	-@mkdir $(dir $@) 
	g++ \
		-dynamiclib \
		-fno-rtti \
		-o $@ \
		$< 

else
ifeq ($(UNAME), Linux)

LINUX_JDK_HOME:=/usr/lib/jvm/java-7-openjdk-amd64

all: natives/linux/libJMumbleLibrary64.so natives/linux/libJMumbleLibrary.so

32: natives/linux/libJMumbleLibrary.so

64: natives/linux/libJMumbleLibrary64.so

build/JMumbleLibrary.o: src/JMumbleLibrary.cpp 
	g++ \
	-m32 \
	-fPIC \
	-fno-rtti \
	"-I${LINUX_JDK_HOME}/include/" \
	-c $< \
	-o $@

build/JMumbleLibrary64.o: src/JMumbleLibrary.cpp 
	g++ \
	-m64 \
	-fPIC \
	-fno-rtti \
	"-I${LINUX_JDK_HOME}/include/" \
	-c $< \
	-o $@

natives/linux/libJMumbleLibrary.so: build/JMumbleLibrary.o
	g++ \
		-m32 \
		-fno-rtti \
		-shared \
		-o $@ \
		"-I${LINUX_JDK_HOME}/include/" \
		$< \
		-lrt

natives/linux/libJMumbleLibrary64.so: build/JMumbleLibrary64.o
	g++ \
		-m64 \
		-fno-rtti \
		-shared \
		-o $@ \
		"-I${LINUX_JDK_HOME}/include/" \
		$< \
		-lrt

endif
endif

clean:
	rm -f build/*.o

