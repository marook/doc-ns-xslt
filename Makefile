CC_OPTS=-Wall -Werror -std=c99
CC_INC=-I /usr/include/libxml2

doc-ns-xslt: doc-ns-xslt.o
	gcc $(CC_OPTS) -o $@ $^ -lxml2 -lxslt

doc-ns-xslt.o: doc-ns-xslt.c
	gcc $(CC_OPTS) $(CC_INC) -c -o $@ $^

