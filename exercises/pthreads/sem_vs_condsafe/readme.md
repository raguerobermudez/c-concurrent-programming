### hello_order_cond_safe example
#### 1° test
<pre>
[randylaptop@RandyLaptop sem_vs_condsafe]$ make clean && make && make run
rm -rf bin/ build/ doc/
mkdir -p build/
gcc -c -Wall -Wextra -pthread -g -std=c17 -I. -I./build -MMD hello_order_cond_safe.c -o build/hello_order_cond_safe.o
mkdir -p bin/
gcc -Wall -Wextra -pthread -g -I. -I./bin -I./build build/hello_order_cond_safe.o -o bin/sem_vs_condsafe -lzip
bin/sem_vs_condsafe
Hello from main thread
Hello from secondary thread 0 of 8
Hello from secondary thread 1 of 8
Hello from secondary thread 2 of 8
Hello from secondary thread 3 of 8
Hello from secondary thread 4 of 8
Hello from secondary thread 5 of 8
Hello from secondary thread 6 of 8
Hello from secondary thread 7 of 8
Execution time: 0.000252547s
</pre>
#### 2° test
<pre>
[randylaptop@RandyLaptop sem_vs_condsafe]$ make clean && make && make run
rm -rf bin/ build/ doc/
mkdir -p build/
gcc -c -Wall -Wextra -pthread -g -std=c17 -I. -I./build -MMD hello_order_cond_safe.c -o build/hello_order_cond_safe.o
mkdir -p bin/
gcc -Wall -Wextra -pthread -g -I. -I./bin -I./build build/hello_order_cond_safe.o -o bin/sem_vs_condsafe -lzip
bin/sem_vs_condsafe
Hello from main thread
Hello from secondary thread 0 of 8
Hello from secondary thread 1 of 8
Hello from secondary thread 2 of 8
Hello from secondary thread 3 of 8
Hello from secondary thread 4 of 8
Hello from secondary thread 5 of 8
Hello from secondary thread 6 of 8
Hello from secondary thread 7 of 8
Execution time: 0.000289590s
</pre>
#### 3° test
<pre>
[randylaptop@RandyLaptop sem_vs_condsafe]$ make clean && make && make run
rm -rf bin/ build/ doc/
mkdir -p build/
gcc -c -Wall -Wextra -pthread -g -std=c17 -I. -I./build -MMD hello_order_cond_safe.c -o build/hello_order_cond_safe.o
mkdir -p bin/
gcc -Wall -Wextra -pthread -g -I. -I./bin -I./build build/hello_order_cond_safe.o -o bin/sem_vs_condsafe -lzip
bin/sem_vs_condsafe
Hello from main thread
Hello from secondary thread 0 of 8
Hello from secondary thread 1 of 8
Hello from secondary thread 2 of 8
Hello from secondary thread 3 of 8
Hello from secondary thread 4 of 8
Hello from secondary thread 5 of 8
Hello from secondary thread 6 of 8
Hello from secondary thread 7 of 8
Execution time: 0.000416798s
</pre>

#### Best time: 1° test: Execution time: 0.000252547s

### hello_order_semaphore example
#### 1° test
<pre>
[randylaptop@RandyLaptop sem_vs_condsafe]$ make clean && make && make run
rm -rf bin/ build/ doc/
mkdir -p build/
gcc -c -Wall -Wextra -pthread -g -std=c17 -I. -I./build -MMD hello_order_semaphor.c -o build/hello_order_semaphor.o
mkdir -p bin/
gcc -Wall -Wextra -pthread -g -I. -I./bin -I./build build/hello_order_semaphor.o -o bin/sem_vs_condsafe -lzip
bin/sem_vs_condsafe
Hello from secondary thread 0 of 8
Hello from secondary thread 1 of 8
Hello from secondary thread 2 of 8
Hello from secondary thread 3 of 8
Hello from secondary thread 4 of 8
Hello from secondary thread 5 of 8
Hello from main thread
Hello from secondary thread 6 of 8
Hello from secondary thread 7 of 8
Execution time: 0.000618508s
</pre>
#### 2° test
<pre>
[randylaptop@RandyLaptop sem_vs_condsafe]$ make clean && make && make run
rm -rf bin/ build/ doc/
mkdir -p build/
gcc -c -Wall -Wextra -pthread -g -std=c17 -I. -I./build -MMD hello_order_semaphor.c -o build/hello_order_semaphor.o
mkdir -p bin/
gcc -Wall -Wextra -pthread -g -I. -I./bin -I./build build/hello_order_semaphor.o -o bin/sem_vs_condsafe -lzip
bin/sem_vs_condsafe
Hello from secondary thread 0 of 8
Hello from secondary thread 1 of 8
Hello from secondary thread 2 of 8
Hello from main thread
Hello from secondary thread 3 of 8
Hello from secondary thread 4 of 8
Hello from secondary thread 5 of 8
Hello from secondary thread 6 of 8
Hello from secondary thread 7 of 8
Execution time: 0.000365878s
</pre>
#### 3° test
<pre>
[randylaptop@RandyLaptop sem_vs_condsafe]$ make clean && make && make run
rm -rf bin/ build/ doc/
mkdir -p build/
gcc -c -Wall -Wextra -pthread -g -std=c17 -I. -I./build -MMD hello_order_semaphor.c -o build/hello_order_semaphor.o
mkdir -p bin/
gcc -Wall -Wextra -pthread -g -I. -I./bin -I./build build/hello_order_semaphor.o -o bin/sem_vs_condsafe -lzip
bin/sem_vs_condsafe
Hello from secondary thread 0 of 8
Hello from secondary thread 1 of 8
Hello from secondary thread 2 of 8
Hello from secondary thread 3 of 8
Hello from secondary thread 4 of 8
Hello from secondary thread 5 of 8
Hello from secondary thread 6 of 8
Hello from main thread
Hello from secondary thread 7 of 8
Execution time: 0.000445528s
[randylaptop@RandyLaptop sem_vs_condsafe]$
</pre>
#### Best time: 2° test: Execution time: 0.000365878s
### Explanation about this test
The conditional safety implementation uses conditional variables control the execution order of threads. This prevents threads from actively blocking and waiting, which could result in higher efficiency in terms of execution time.

The semaphore implementation may involve a more complex mechanism of locking and unlocking, which could lead to longer execution times. Therefore, in this particular example, the conditional safety implementation had better performance.
