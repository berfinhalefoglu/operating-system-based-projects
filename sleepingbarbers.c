#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CHAIRS 5        // Bekleme koltuğu sayısı
#define CUSTOMERS 10    // Salona gelecek toplam müşteri sayısı
#define BARBERS 2       // Aynı anda çalışabilen berber sayısı

sem_t waitingChairs;         // boş koltuk sayısı
sem_t barberReady;           // berber hazır mı
pthread_mutex_t mutex;       // bekleme koltuğu değişimini kilitlemek için

void* customer(void* arg) {
    int id = *(int*)arg;

    printf("Müşteri %d salona geldi.\n", id);
    fflush(stdout);
     // Bekleme koltuğu varsa müşteri oturur ve berberi uyandırır
    if (sem_trywait(&waitingChairs) == 0) {
        printf("Müşteri %d bekleme koltuğuna oturdu.\n", id);
        fflush(stdout);
        sem_post(&barberReady);  // berberi uyandır
    } else {
         // Bekleme koltuğu yoksa müşteri gider
        printf("Müşteri %d yer bulamadı, gitti.\n", id);
        fflush(stdout);
    }

    return NULL;
}
void* barber(void* arg) {
    int id = *(int*)arg; // Berber ID’si

    while (1) {
        sem_wait(&barberReady);  // Müşteri gelene kadar bekler

        pthread_mutex_lock(&mutex);      // Koltuk sayısını güvenli şekilde değiştirmek için kilitler
        sem_post(&waitingChairs);        // Müşteri alınır, koltuk boşalır
        printf("Berber %d bir müşteriyi tıraş ediyor.\n", id);
        fflush(stdout);
        pthread_mutex_unlock(&mutex);    // Kilidi bırakır

        sleep(2); // Tıraş süresi
    }

    return NULL;
}

int main() {
    pthread_t barberThreads[BARBERS];
    pthread_t customerThreads[CUSTOMERS];
    int ids[BARBERS > CUSTOMERS ? BARBERS : CUSTOMERS];

    sem_init(&waitingChairs, 0, CHAIRS);
    sem_init(&barberReady, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    // Berber iş parçacıkları başlatılır
    for (int i = 0; i < BARBERS; i++) {
        ids[i] = i;
        pthread_create(&barberThreads[i], NULL, barber, &ids[i]);
    }
    // Müşteriler 1 saniye arayla salona gelir
    for (int i = 0; i < CUSTOMERS; i++) {
        ids[i] = i;
        pthread_create(&customerThreads[i], NULL, customer, &ids[i]);
        sleep(1); // her müşteri 1 sn arayla gelir
    }
    // Tüm müşteriler işini bitirene kadar ana iş parçacığı bekler
    for (int i = 0; i < CUSTOMERS; i++) {
        pthread_join(customerThreads[i], NULL);
    }

    return 0;
}
