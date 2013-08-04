UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)

all: natives/osx/libJMumbleLibrary64.jnilib natives/osx/libJMumbleLibrary.jnilib

build/JMumbleLibrary.o: src/JMumbleLibrary.cpp
	g++ \
	-fno-rtti \
	-m32 \
	"-I/System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers" \
	"-I/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers" \
	-c $< \
	-o $@

build/JMumbleLibrary64.o: JMumbleLibrary.cpp
	g++ \
	-fno-rtti \
	"-I/System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers" \
	"-I/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers" \
	-c $< \
	-o $@

natives/osx/libJMumbleLibrary.jnilib: build/JMumbleLibrary.o
	g++ \
		-dynamiclib \
		-fno-rtti \
		-m32 \
		-o $@ \
		$< 

natives/osx/libJMumbleLibrary64.jnilib: build/JMumbleLibrary64.o
	g++ \
		-dynamiclib \
		-fno-rtti \
		-o $@ \
		$< 

else
ifeq ($(UNAME), Linux)


all: natives/linux/libJMumbleLibrary64.so natives/linux/libJMumbleLibrary.so


build/JMumbleLibrary.o: src/JMumbleLibrary.cpp
	-@mkdir $(dir $@) 
	g++ \
	-m32 \
	-fPIC \
	-fno-rtti \
	"-I/usr/lib/jvm/java-6-openjdk-amd64/include/" \
	-c $< \
	-o $@

build/JMumbleLibrary64.o: src/JMumbleLibrary.cpp
	-@mkdir $(dir $@) 
	g++ \
	-fPIC \
	-fno-rtti \
	"-I/usr/lib/jvm/java-6-openjdk-amd64/include/" \
	-c $< \
	-o $@

natives/linux/libJMumbleLibrary.so: build/JMumbleLibrary.o
	-@mkdir -p $(dir $@) 
	g++ \
		-m32 \
		-fno-rtti \
		-shared \
		-o $@ \
		"-I/usr/lib/jvm/java-6-openjdk-amd64/include/" \
		$< 

natives/linux/libJMumbleLibrary64.so: build/JMumbleLibrary64.o
	-@mkdir -p $(dir $@) 
	g++ \
		-fno-rtti \
		-shared \
		-o $@ \
		"-I/usr/lib/jvm/java-6-openjdk-amd64/include/" \
		$< 

endif
endif

clean:
	rm -f build/*.o

