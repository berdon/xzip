ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
EXECUTABLE := xzip
DIR_SRC := ${ROOT_DIR}/src
DIR_BUILD := ${ROOT_DIR}/build
DIR_OUT := ${ROOT_DIR}/out
DIR_PLAY_BUILD := ${ROOT_DIR}/play-build
DIR_PLAY_OUT := ${ROOT_DIR}/play-out

# play:
# 	mkdir -p ${DIR_PLAY_BUILD}
# 	mkdir -p ${DIR_PLAY_OUT}
# 	cd ${DIR_PLAY_BUILD} && clang++ -g -std=c++20 -c \
# 		${ROOT_DIR}/play-src/**.cpp
# 	clang++ -g1 -rdynamic -std=c++20 ${DIR_PLAY_BUILD}/*.o -o ${DIR_PLAY_OUT}/play

xzip:
	mkdir -p ${DIR_BUILD}
	mkdir -p ${DIR_OUT}
	cd ${DIR_BUILD} && clang++ -g -std=c++20 -c \
		${ROOT_DIR}/src/**.cpp \
		${ROOT_DIR}/src/**/*.cpp \
		${ROOT_DIR}/src/xtd/compression/*.cpp \
		${ROOT_DIR}/src/gzip/deflate/*.cpp
	clang++ -g1 -rdynamic -std=c++20 ${DIR_BUILD}/*.o -o ${DIR_OUT}/${EXECUTABLE}

clean:
	rm -rf ${DIR_BUILD}
	rm -rf ${DIR_OUT}