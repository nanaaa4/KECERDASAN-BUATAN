#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CITIES 30
#define MAX_PATH_LENGTH 30
#define MAX_NAME_LENGTH 20
#define MAX_LOG_ENTRIES 100
#define MAX_NEIGHBORS 10

// Struktur untuk representasi graf
typedef struct {
    char name[MAX_NAME_LENGTH];
    char neighbors[MAX_NEIGHBORS][MAX_NAME_LENGTH];
    int neighbor_count;
} City;

// Struktur untuk menyimpan path
typedef struct {
    char cities[MAX_PATH_LENGTH][MAX_NAME_LENGTH];
    int length;
} Path;

// Struktur queue untuk BFS
typedef struct {
    Path paths[MAX_CITIES * MAX_CITIES];
    int front;
    int rear;
} Queue;

// Inisialisasi queue
void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
}

// Cek apakah queue kosong
bool isQueueEmpty(Queue *q) {
    return q->rear < q->front;
}

// Menambahkan path ke queue
void enqueue(Queue *q, Path path) {
    q->rear++;
    q->paths[q->rear] = path;
}

// Mengambil path dari queue
Path dequeue(Queue *q) {
    Path path = q->paths[q->front];
    q->front++;
    return path;
}

// Fungsi untuk memeriksa apakah kota sudah dikunjungi
bool isVisited(char visited[][MAX_NAME_LENGTH], int visit_count, char *city) {
    for (int i = 0; i < visit_count; i++) {
        if (strcmp(visited[i], city) == 0) {
            return true;
        }
    }
    return false;
}

// Fungsi untuk mencari indeks kota dalam array kota
int findCityIndex(City cities[], int city_count, char *name) {
    for (int i = 0; i < city_count; i++) {
        if (strcmp(cities[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Implementasi BFS untuk menemukan path dengan melacak kota yang dikunjungi
Path bfs_find_path(City cities[], int city_count, char *start, char *end, 
                  char visited[][MAX_NAME_LENGTH], int *visit_count,
                  char log[][MAX_NAME_LENGTH + 15], int *log_count) {
    // Cek apakah kota asal dan tujuan ada dalam graf
    int start_idx = findCityIndex(cities, city_count, start);
    int end_idx = findCityIndex(cities, city_count, end);
    // Inisialisasi path kosong untuk hasil yang gagal
    Path empty_path;
    empty_path.length = 0;
    
    if (start_idx == -1 || end_idx == -1) {
        sprintf(log[0], "Error: '%s' atau '%s' tidak ditemukan dalam peta.", start, end);
        *log_count = 1;
        return empty_path;
    }

    // Inisialisasi queue untuk BFS
    Queue queue;
    initQueue(&queue);
    
    // Reset visited array dan counter
    *visit_count = 0;
    
    // Buat path awal dengan kota asal
    Path initial_path;
    initial_path.length = 1;
    strcpy(initial_path.cities[0], start);
    enqueue(&queue, initial_path);
    
    // Tandai kota asal sebagai dikunjungi
    strcpy(visited[*visit_count], start);
    (*visit_count)++;
    
    *log_count = 0;
    
    // Proses BFS
    while (!isQueueEmpty(&queue)) {
        Path current_path = dequeue(&queue);
        char *current_city = current_path.cities[current_path.length - 1];
        
        // Catat log kunjungan
        sprintf(log[*log_count], "Mengunjungi: %s", current_city);
        (*log_count)++;
        
        // Jika sudah sampai tujuan
        if (strcmp(current_city, end) == 0) {
            return current_path;  // Path ditemukan
        }
        
        // Cari indeks kota saat ini
        int city_idx = findCityIndex(cities, city_count, current_city);
        
        // Kunjungi semua tetangga
        for (int i = 0; i < cities[city_idx].neighbor_count; i++) {
            char *neighbor = cities[city_idx].neighbors[i];
            
            // Hanya kunjungi kota yang belum dikunjungi
            if (!isVisited(visited, *visit_count, neighbor)) {
                // Tandai sebagai dikunjungi
                strcpy(visited[*visit_count], neighbor);
                (*visit_count)++;
                
                // Buat jalur baru dengan menambahkan tetangga
                Path new_path = current_path;
                strcpy(new_path.cities[new_path.length], neighbor);
                new_path.length++;
                
                // Tambahkan jalur baru ke queue
                enqueue(&queue, new_path);
            }
        }
    }
    
    // Jika tidak ada path ditemukan
    return empty_path;
}

int main() {
    // Inisialisasi graf (peta)
    City cities[MAX_CITIES];
    int city_count = 0;
    
    // Arad
    strcpy(cities[city_count].name, "Arad");
    strcpy(cities[city_count].neighbors[0], "Timisoara");
    strcpy(cities[city_count].neighbors[1], "Sibiu");
    strcpy(cities[city_count].neighbors[2], "Zerind");
    cities[city_count].neighbor_count = 3;
    city_count++;
    
    // Timisoara
    strcpy(cities[city_count].name, "Timisoara");
    strcpy(cities[city_count].neighbors[0], "Lugoj");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Lugoj
    strcpy(cities[city_count].name, "Lugoj");
    strcpy(cities[city_count].neighbors[0], "Mehadia");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Mehadia
    strcpy(cities[city_count].name, "Mehadia");
    strcpy(cities[city_count].neighbors[0], "Dobreta");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Dobreta
    strcpy(cities[city_count].name, "Dobreta");
    strcpy(cities[city_count].neighbors[0], "Craiova");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Sibiu
    strcpy(cities[city_count].name, "Sibiu");
    strcpy(cities[city_count].neighbors[0], "Rimicu Vilea");
    strcpy(cities[city_count].neighbors[1], "Fagaras");
    cities[city_count].neighbor_count = 2;
    city_count++;
    
    // Rimicu Vilea
    strcpy(cities[city_count].name, "Rimicu Vilea");
    strcpy(cities[city_count].neighbors[0], "Craiova");
    strcpy(cities[city_count].neighbors[1], "Pitesti");
    cities[city_count].neighbor_count = 2;
    city_count++;
    
    // Craiova
    strcpy(cities[city_count].name, "Craiova");
    strcpy(cities[city_count].neighbors[0], "Dobreta");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Pitesti
    strcpy(cities[city_count].name, "Pitesti");
    strcpy(cities[city_count].neighbors[0], "Craiova");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Fagaras
    strcpy(cities[city_count].name, "Fagaras");
    strcpy(cities[city_count].neighbors[0], "Bucharest");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Bucharest
    strcpy(cities[city_count].name, "Bucharest");
    strcpy(cities[city_count].neighbors[0], "Giurgiu");
    strcpy(cities[city_count].neighbors[1], "Urziceni");
    cities[city_count].neighbor_count = 2;
    city_count++;
    
    // Giurgiu
    strcpy(cities[city_count].name, "Giurgiu");
    cities[city_count].neighbor_count = 0;
    city_count++;
    
    // Urziceni
    strcpy(cities[city_count].name, "Urziceni");
    strcpy(cities[city_count].neighbors[0], "Hirsova");
    strcpy(cities[city_count].neighbors[1], "Vaslui");
    cities[city_count].neighbor_count = 2;
    city_count++;
    
    // Hirsova
    strcpy(cities[city_count].name, "Hirsova");
    strcpy(cities[city_count].neighbors[0], "Eforie");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Eforie
    strcpy(cities[city_count].name, "Eforie");
    cities[city_count].neighbor_count = 0;
    city_count++;
    
    // Vaslui
    strcpy(cities[city_count].name, "Vaslui");
    strcpy(cities[city_count].neighbors[0], "Lasi");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Lasi
    strcpy(cities[city_count].name, "Lasi");
    strcpy(cities[city_count].neighbors[0], "Neamt");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Neamt
    strcpy(cities[city_count].name, "Neamt");
    cities[city_count].neighbor_count = 0;
    city_count++;
    
    // Zerind
    strcpy(cities[city_count].name, "Zerind");
    strcpy(cities[city_count].neighbors[0], "Oradea");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Oradea
    strcpy(cities[city_count].name, "Oradea");
    strcpy(cities[city_count].neighbors[0], "Sibiu");
    cities[city_count].neighbor_count = 1;
    city_count++;
    
    // Input kota asal dan tujuan
    char start_city[MAX_NAME_LENGTH];
    char end_city[MAX_NAME_LENGTH];
    
    printf("Masukkan kota asal: ");
    scanf("%s", start_city);
    
    printf("Masukkan kota tujuan: ");
    scanf("%s", end_city);
    
    // Variabel untuk menyimpan path, visited, dan log
    char visited[MAX_CITIES][MAX_NAME_LENGTH];
    int visit_count = 0;
    char log[MAX_LOG_ENTRIES][MAX_NAME_LENGTH + 15];
    int log_count = 0;
    
    // Jalankan BFS untuk mencari path
    Path result_path = bfs_find_path(cities, city_count, start_city, end_city, visited, &visit_count, log, &log_count);
    
    // Tampilkan hasil path
    printf("\nPath from %s to %s:\n", start_city, end_city);
    if (result_path.length > 0) {
        printf("%s", result_path.cities[0]);
        for (int i = 1; i < result_path.length; i++) {
            printf(" -> %s", result_path.cities[i]);
        }
        printf("\n");
    } else {
        printf("No path found.\n");
    }
    
    // Tampilkan semua kota yang dikunjungi selama pencarian
    printf("\nVisited cities during search:\n");
    for (int i = 0; i < visit_count; i++) {
        printf("%s", visited[i]);
        if (i < visit_count - 1) {
            printf(", ");
        }
    }
    printf("\n");
    
    // Tampilkan log kunjungan node
    printf("\nBFS Visited Nodes (in order):\n");
    for (int i = 0; i < log_count; i++) {
        printf("%s", log[i]);
        if (i < log_count - 1) {
            printf(", ");
        }
    }
    printf("\n");
    
    return 0;
}