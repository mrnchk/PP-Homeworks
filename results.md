### Результаты на сете из ~500 элементов  
  
## Грубая:  
### 1 thread: 1.53959e+06 ops/sec  
### 2 threads: 516369 ops/sec  
### 4 threads: 544990 ops/sec  
### 8 threads: 385264 ops/sec  
### 16 threads: 264122 ops/sec  
  
## Тонкая:  
### 1 thread: 157904 ops/sec  
### 2 threads: 219704 ops/sec  
### 4 threads: 46046.5 ops/sec  
### 8 threads: 42234.5 ops/sec  
### 16 threads: 40498.5 ops/sec  
  
## Оптимистичная:  
### 1 threads: 1.11883e+06 ops/sec  
  validations: 448728  
  failed: 0  
  fail rate: 0%  
### 2 threads: 2.22491e+06 ops/sec  
  validations: 892033  
  failed: 320  
  fail rate: 0.0358731%  
### 4 threads: 4.28808e+06 ops/sec  
  validations: 1717656  
  failed: 2038  
  fail rate: 0.11865%  
### 8 threads: 7.19566e+06 ops/sec  
  validations: 2887677  
  failed: 8393  
  fail rate: 0.290649%  
### 16 threads: 8.24668e+06 ops/sec  
  validations: 3323486  
  failed: 22161  
  fail rate: 0.6668%  
  
## Ленивая:  
### 1 threads: 1.45127e+06 ops/sec  
  validations: 436221  
  failed: 0  
  fail rate: 0%  
### 2 threads: 2.91409e+06 ops/sec  
  validations: 875948  
  failed: 59  
  fail rate: 0.00673556%  
### 4 threads: 5.48321e+06 ops/sec  
  validations: 1644635  
  failed: 451  
  fail rate: 0.0274225%  
### 8 threads: 8.81366e+06 ops/sec  
  validations: 2645446  
  failed: 2153  
  fail rate: 0.0813851%  
### 16 threads: 1.0376e+07 ops/sec  
  validations: 3119970  
  failed: 6436  
  fail rate: 0.206284%  

# Вывод
Тонкая синхронизация показала себя хуже остальных, так как lock на чтение в такой задаче занимает слишком много времени, отставание было бы не таким большим при большем числе потоков, разница с грубой уменьшается с их увеличением  
Ленивая синхронизация самая эффективная при любом числе потоков, она(и оптимистичная) выигрывает за счёт того, что операций чтения в разы больше, проваленных валидаций за счёт этого тоже не очень много, а на самом чтении блокировка даже не берётся