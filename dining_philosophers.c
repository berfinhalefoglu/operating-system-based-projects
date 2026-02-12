#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

#define N 5   // Filozof sayısı ve aynı zamanda çubuk (chopstick) sayısı

sem_t room;         // Yöntem 1 için en fazla N-1 filozof aynı anda masada olabilir
sem_t chopsticks[N]; // Her filozofun solunda bir çubuk (semafor)


void* philosopher(void* num) {
    int id = *(int*)num;

    sem_wait(&room); // Maksimum N-1 filozofun masaya oturmasını sağlar

    printf("Philosopher %d is thinking.\n", id);
    fflush(stdout);
    sleep(1);
     // Sol ve sağ çubuğu alır
    sem_wait(&chopsticks[id]);
    sem_wait(&chopsticks[(id + 1) % N]);

    printf("Philosopher %d is eating.\n", id);
    fflush(stdout);
    sleep(2);
     // Yemeğini bitirir, çubukları bırakır
    sem_post(&chopsticks[id]);
    sem_post(&chopsticks[(id + 1) % N]);

    printf("Philosopher %d finished eating and left.\n", id);
    fflush(stdout);
    sem_post(&room);

    return NULL;
}
// Yalnızca iki çubuk da müsaitse yemeğe başlar. Aksi halde kısa bir süre bekler ve tekrar dener.
// Bu şekilde deadlock riski en aza indirgenmiş olur.


void* philosopher_method2(void* num) {
    int id = *(int*)num;

    printf("Philosopher %d is thinking.\n", id);
    fflush(stdout);
    sleep(1);

    while (true) {
        if (sem_trywait(&chopsticks[id]) == 0) {
            if (sem_trywait(&chopsticks[(id + 1) % N]) == 0) {
                printf("Philosopher %d is eating.\n", id);
                fflush(stdout);
                sleep(2);
                printf("Philosopher %d finished eating and left.\n", id);
                fflush(stdout);

                sem_post(&chopsticks[id]);
                sem_post(&chopsticks[(id + 1) % N]);
                break;
            } else {
                sem_post(&chopsticks[id]);
            }
        }
        usleep(100);
    }

    return NULL;
}

void* philosopher_method3(void* num) {
    int id = *(int*)num;

    printf("Philosopher %d is thinking.\n", id);
    fflush(stdout);
    sleep(1);

    if (id % 2 == 0) {
        sem_wait(&chopsticks[(id + 1) % N]);
        sem_wait(&chopsticks[id]);
    } else {
        sem_wait(&chopsticks[id]);
        sem_wait(&chopsticks[(id + 1) % N]);
    }

    printf("Philosopher %d is eating.\n", id);
    fflush(stdout);
    sleep(2);

    sem_post(&chopsticks[id]);
    sem_post(&chopsticks[(id + 1) % N]);

    printf("Philosopher %d finished eating and left.\n", id);
    fflush(stdout);

    return NULL;
}

void* philosopher_method4(void* num) {
    int id = *(int*)num;

    printf("Philosopher %d is thinking.\n", id);
    fflush(stdout);
    sleep(1);

    if (id == 2) { // Örnek: Philosopher 2 daima sağ çubuğu önce alır
        sem_wait(&chopsticks[(id + 1) % N]);
        sem_wait(&chopsticks[id]);
    } else {
        sem_wait(&chopsticks[id]);
        sem_wait(&chopsticks[(id + 1) % N]);
    }

    printf("Philosopher %d is eating.\n", id);
    fflush(stdout);
    sleep(2);

    sem_post(&chopsticks[id]);
    sem_post(&chopsticks[(id + 1) % N]);

    printf("Philosopher %d finished eating and left.\n", id);
    fflush(stdout);

    return NULL;
}

int main() {
    int method;
    scanf("%d", &method);

    pthread_t threads[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        sem_init(&chopsticks[i], 0, 1);
        ids[i] = i;
    }

    if (method == 1) {
    sem_init(&room, 0, N - 1); // En fazla N-1 filozof aynı anda yemek yiyebilir (deadlock önleme)
    for (int i = 0; i < N; i++)
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }
    else if (method == 2) {
        // Filozoflar sadece iki çubuğu birden bulabildiklerinde yemek yer (sem_trywait)
        for (int i = 0; i < N; i++)
            pthread_create(&threads[i], NULL, philosopher_method2, &ids[i]);
    }
    else if (method == 3) {
        // Asimetrik çözüm: tek/çift filozoflar farklı çubuk alma sırası izler
        for (int i = 0; i < N; i++)
            pthread_create(&threads[i], NULL, philosopher_method3, &ids[i]);
    }
    else if (method == 4) {
        // Özel çözüm: yalnızca filozof 2 sağ çubuğu önce alır, diğerleri standart şekilde davranır
        for (int i = 0; i < N; i++)
            pthread_create(&threads[i], NULL, philosopher_method4, &ids[i]);
    }
    else {
        // Geçersiz giriş kontrolü
        printf("Only method 1 to 4 are implemented.\n");
        fflush(stdout);
        return 0;
    }


    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    return 0;
}
