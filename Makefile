
############################################################################################### make + install
make:
	@echo "SCC is headers only code, no preliminary compilation is needed."
	@echo "After checking out from repo, it's in read-to-use state."

PREFIX=/usr/local
install:
	@echo "not implemented - just use this (sto/) directory."
#	mkdir -p  ${PREFIX}/bin/
#	cp -v sto ${PREFIX}/bin/
#	mkdir -p  ${PREFIX}/include/sto/
#	cp -v *.h ${PREFIX}/include/sto/


############################################################################################### asciidoc gen
ASCIIDOC_FILTERS ?= /usr/share/asciidoc/filters


ifeq ($(USER),lvv)
	HOMEDIR := /home/lvv/p/volnitsky.com/
	INCLUDE := $(HOMEDIR)/include.mk 
else
	INCLUDE := /dev/null
endif


include $(INCLUDE)

index.txt: 
	head -n-1 README.asciidoc > /tmp/t.ad
show:
	make -B index.html
	google-chrome index.html &

u-meta.cc: *.h

#COPY_LIST += $(wildcard *.png)


###########################################################


CLEAN_LIST += t-print  t-regex t-meta

#CXXFLAGS +=   -std=gnu++11 -Wall -I/home/lvv/p/ 
CXXFLAGS +=   -Wall -I/home/lvv/p/ -I .. 

t-buf-r: t-buf

########################################################  SCCPP

######################################################  Unit Tests
CXXFLAGS+= -std=gnu++11 -Wall -O0 -ggdb3 -D_GLIBCXX_DEBUG  -fno-inline -I/home/lvv/p/  
