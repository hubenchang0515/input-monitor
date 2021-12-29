input-monitor : monitor.o input_event.o main.o  
	gcc -o $@ $^  

monitor.o: monitor.c monitor.h
	gcc -c monitor.c  

input_event.o: input_event.c input_event.h input_event_code_name.h
	gcc -c input_event.c  

main.o: main.c input_event.h monitor.h
	gcc -c main.c  

clean:
	rm -f monitor.o input_event.o main.o