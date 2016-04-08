com_filter: com_filter.c com_main.c
	gcc com_filter.c com_main.c -o com_filter
clean:
	rm -f *.o *~ com_filter

