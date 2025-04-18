# A Simple Makefile to easily run examples/test source code.
# For only Header Libraries with my basic repository structure.
#
# Usage :
#		make sample
#		make clean
#		make static/lib
#		make dynamic/dll

CC = gcc
CCFLAGS = -Wall -Wextra -std=c23 -Wformat -Wempty-body
CCWARNS = -Wunused-function -Wempty-body -Wunused-variable
STATIC =  -c -o cprof.lib
DYNAMIC= -shared -o cprof.dll

# Comment/Uncomment to run examples/tests
FOLDER = ./examples

# Comment/Uncomment to run examples/tests
# FOLDER = ./tests

all : static dynamic
lib : static 
dll : dynamic

% :
	@$(CC) $(CCFLAGS) $(CCWARNS) $(FOLDER)/$@.c  cprofiler.c -I.  -o $@ 
	@./$@

static:
	$(CC) $(CCFLAGS) cprofiler.c $(STATIC)

dynamic:
	$(CC) $(CCFLAGS) cprofiler.c $(DYNAMIC)

run:
	@./a

clean:
	@rm -f ./*.o
	@rm -f ./*.exe

.Phony : all clean

# ZZZZZZZZZZZZZZZZZZZ  EEEEEEEEEEEEEEEEEEEEEE  KKKKKKKKK    KKKKKKK  EEEEEEEEEEEEEEEEEEEEEE
# Z:::::::::::::::::Z  E::::::::::::::::::::E  K:::::::K    K:::::K  E::::::::::::::::::::E
# Z:::::::::::::::::Z  E::::::::::::::::::::E  K:::::::K    K:::::K  E::::::::::::::::::::E
# Z:::ZZZZZZZZ:::::Z   EE::::::EEEEEEEEE::::E  K:::::::K   K::::::K  EE::::::EEEEEEEEE::::E
# ZZZZZ     Z:::::Z      E:::::E       EEEEEE  KK::::::K  K:::::KKK    E:::::E       EEEEEE
#         Z:::::Z        E:::::E                K:::::K K:::::K        E:::::E
#        Z:::::Z         E::::::EEEEEEEEEE      K::::::K:::::K         E::::::EEEEEEEEEE
#       Z:::::Z          E:::::::::::::::E      K:::::::::::K          E:::::::::::::::E
#      Z:::::Z           E:::::::::::::::E      K:::::::::::K          E:::::::::::::::E
#     Z:::::Z            E::::::EEEEEEEEEE      K::::::K:::::K         E::::::EEEEEEEEEE
#    Z:::::Z             E:::::E                K:::::K K:::::K        E:::::E
# ZZZ:::::ZZZZZZZZZZZ    E:::::E       EEEEEE  KK::::::K  K:::::KKK    E:::::E       EEEEEE
# Z:::::::::::::::::Z  E::::::::::::::::::::E  K:::::::K   K::::::K  E::::::::::::::::::::E
# Z:::::::::::::::::Z  E::::::::::::::::::::E  K:::::::K    K:::::K  E::::::::::::::::::::E
# ZZZZZZZZZZZZZZZZZZZ  EEEEEEEEEEEEEEEEEEEEEE  KKKKKKKKK    KKKKKKK  EEEEEEEEEEEEEEEEEEEEEE
