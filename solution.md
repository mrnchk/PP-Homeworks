## Задача 1
Если предполагается, что Петя и Вася всегда следят за реактором и сообщениями, если они не на обеде достаточно два сообщения  
-Я ушёл на обед  
-Я вернулся с обеда  
Помимо этого заведём правило: "Если оба одновременно отправили сообщение о том, что уходят на обед, первым идёт Петя"  
Если не гарантируется постоянное наблюдение, и возможны какие-то происшествия, тогда требуем ответа о том, что Вася увидел, что Петя уходит на обед, или наоборот  

---

## Задача 2

### Максимальное значение

Если потоки выполняют операции без конфликтов, каждый из 100 потоков выполнит 100 инкрементов  
Тогда максимальное значение - 10000

### Минимальное значение

Для начала докажем нижнюю границу, 1 получить не можем так какой-то поток на последней своей итерации прочитает count = 1 и увеличит его на один  
Как получить 2:  
первый поток записывает temp = 0 и останавливается, 98 потоков делают все свои итерации, последний поток делает все итерации кроме одной  
первый поток записывает count = 1 и затирает все остальные инкременты  
последний поток записывает temp = 1 и останавливается  
первый поток делает все свои итерации  
последний поток записывает count = 2  
Минимальное значение - 2

---

## Задача 3.1
```
counting_semaphore<N> leaders;  
counting_semaphore<N> followers;  
  
void dance()  
{
    // ...
}

void add_leader()
{
    leaders.release();
    followers.acquire();

    dance();
}

void add_follower()
{
    followers.release();
    leaders.acquire();

    dance();
}
```

## Задача 3.2
```
mutex m;

queue<binary_semaphore*> waitingLeaders;
queue<binary_semaphore*> waitingFollowers;

void dance() {
    // ...
}

void add_leader() {
    binary_semaphore ready(0);
    binary_semaphore* partner = nullptr;

    {
        lock_guard<mutex> lock(m);

        if (!waitingFollowers.empty()) {
            partner = waitingFollowers.front();
            waitingFollowers.pop();
        } else {
            waitingLeaders.push(&ready);
        }
    }

    if (partner)
        partner->release();
    else
        ready.acquire();

    dance();
}

void add_follower() {
    binary_semaphore ready(0);
    binary_semaphore* partner = nullptr;

    {
        lock_guard<mutex> lock(m);

        if (!waitingLeaders.empty()) {
            partner = waitingLeaders.front();
            waitingLeaders.pop();
        } else {
            waitingFollowers.push(&ready);
        }
    }

    if (partner)
        partner->release();
    else
        ready.acquire();

    dance();
}
```
