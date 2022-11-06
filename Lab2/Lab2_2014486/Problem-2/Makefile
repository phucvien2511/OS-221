all:
	g++ -o pi_serial pi_serial.cpp -lgomp
	g++ -o pi_multi-thread pi_threads.cpp -lgomp
pi_serial:
	g++ -o pi_serial pi_serial.cpp -lgomp
pi_multi-thread:
	g++ -o pi_multi-thread pi_threads.cpp -lgomp
clean:
	rm pi_serial && rm pi_multi-thread
