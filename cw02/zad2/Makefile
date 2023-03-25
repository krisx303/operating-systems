MEASURE_FILE=test_file.txt
REVERSED=reversed
REVERSED_2=reversed2

all: measure


compile_main: main.c
	gcc -Wall main.c -o main_byte -D BUFFER_SIZE=1 -D MEASURES=1
	gcc -Wall main.c -o main_buffer -D MEASURES=1

run_single_test:
	@echo "Running test $(TEST): ($(TEST_DESC))"
	@echo "Command: $(TEST_COMMAND)"
	@if $(TEST_COMMAND) ; then \
		echo "\033[32mTest passed\033[0m"; \
	else \
		echo "\033[31mTest failed\033[0m"; \
	fi

test: compile_main
	# testing input
	@make run_single_test TEST_DESC="zero arguments" TEST_COMMAND="! ./main_buffer" TEST=1 --silent
	@make run_single_test TEST_DESC="too few arguments" TEST_COMMAND="! ./main_buffer Makefile" TEST=2 --silent
	@make run_single_test TEST_DESC="Non existing src file" TEST_COMMAND="! ./main_buffer hello.asd $(REVERSED)" TEST=3 --silent
	
	@make compile_main MEASURES=1 --silent
	@make run_single_test TEST_DESC="Valid input" TEST_COMMAND="./main_buffer main.c $(REVERSED)" TEST=4 --silent
	@make run_single_test TEST_DESC="Checking reverse byte by byte" TEST_COMMAND="./main_byte main.c $(REVERSED) ; ./main_byte $(REVERSED) $(REVERSED_2) ; diff -s main.c $(REVERSED_2)" TEST=5 --silent
	@make run_single_test TEST_DESC="Checking reverse buffer" TEST_COMMAND="./main_buffer main.c $(REVERSED) ; ./main_buffer $(REVERSED) $(REVERSED_2) ; diff -s main.c $(REVERSED_2)" TEST=6 --silent


measure:
	@echo "Running measures (10 times for each buffer)"
	@-rm pomiar_zad2.txt $(REVERSED) $(REVERSED_2)

	@echo "Ran 10 measures for each buffer size" >> pomiar_zad2.txt

	@echo "\nBUFFER SIZE 1" >> pomiar_zad2.txt
	gcc -Wall main.c -o main_byte -D BUFFER_SIZE=1 -D MEASURES=10
	./main_byte $(MEASURE_FILE) reversed >> pomiar_zad2.txt

	@echo "\nBUFFER SIZE 1024" >> pomiar_zad2.txt
	gcc -Wall main.c -o main_buffer -D BUFFER_SIZE=1024 -D MEASURES=10
	./main_buffer $(MEASURE_FILE) reversed >> pomiar_zad2.txt
	@echo "Measures done"

clean:
	rm main_buffer main_byte $(REVERSED_2) $(REVERSED)