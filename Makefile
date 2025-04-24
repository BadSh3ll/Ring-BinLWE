SOURCES = main.c random.c utils.c kem.c poly.c
OUTPUT = main

all: build run

build:
	@gcc -o $(OUTPUT) -Wall $(SOURCES)

run:
	@./$(OUTPUT)

clean:
	@rm -f $(OUTPUT)
	@echo "Cleaned up the build files."
	@echo "Done."