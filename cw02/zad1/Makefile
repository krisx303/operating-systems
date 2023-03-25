MEASURES=10
MEASURE_FILE=test_file.txt
REPLACED=replaced

all: measure


compile_main: main.c
	gcc -Wall main.c -o main_lib -D MEASURES=$(MEASURES)
	gcc -Wall main.c -o main_sys -D SYS -D MEASURES=$(MEASURES)

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
	@make run_single_test TEST_DESC="zero arguments" TEST_COMMAND="! ./main_lib" TEST=1 --silent
	@make run_single_test TEST_DESC="too few arguments" TEST_COMMAND="! ./main_lib a b" TEST=2 --silent
	@make run_single_test TEST_DESC="Invalid char argument" TEST_COMMAND="! ./main_lib hello b hello.txt $(REPLACED)" TEST=3 --silent
	@make run_single_test TEST_DESC="Non existing src file" TEST_COMMAND="! ./main_lib a b nothing.csv $(REPLACED)" TEST=4 --silent
	
	@make run_single_test TEST_DESC="Valid input" TEST_COMMAND="./main_lib a b main.c $(REPLACED)" TEST=5 --silent

	@make run_single_test TEST_DESC="Checking differences lib" TEST_COMMAND="./main_lib T + Makefile $(REPLACED) ;tr T + < Makefile > replaced_tr ; diff -s replaced_tr $(REPLACED)" TEST=6 --silent
	@make run_single_test TEST_DESC="Checking differences sys" TEST_COMMAND="./main_sys T + Makefile $(REPLACED) ;tr T + < Makefile > replaced_tr ; diff -s replaced_tr $(REPLACED)" TEST=7 --silent

run_measure:
	@echo "\nBUFFER SIZE $(BUFFER_SIZE)" >> pomiar_zad1.txt
	@echo "Running measure with $(BUFFER_SIZE) buffer size"
	@gcc -Wall main.c -o main_lib -D BUFFER_SIZE=$(BUFFER_SIZE) -D MEASURES=$(MEASURES)
	@gcc -Wall main.c -o main_sys -D SYS -D BUFFER_SIZE=$(BUFFER_SIZE) -D MEASURES=$(MEASURES)
	@./main_lib T + $(MEASURE_FILE) $(REPLACED) >> pomiar_zad1.txt
	@./main_sys T + $(MEASURE_FILE) $(REPLACED) >> pomiar_zad1.txt

measure:
	@echo "Running measures"
	@rm pomiar_zad1.txt
	@echo "Ran 10 measures for each buffer size" >> pomiar_zad1.txt
	@make run_measure BUFFER_SIZE=1 --silent
	@make run_measure BUFFER_SIZE=8 --silent
	@make run_measure BUFFER_SIZE=32 --silent
	@make run_measure BUFFER_SIZE=1024 --silent
	@make run_measure BUFFER_SIZE=4096 --silent
	@echo "Measures done"


clean:
	rm main_lib main_sys replaced_tr $(REPLACED)