#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NODES 10
#define INF INT_MAX

typedef struct Node {
    int vertex;
    int weight;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} AdjList;

typedef struct {
    int vertex;
    int distance;
} MinHeapNode;

typedef struct {
    MinHeapNode* nodes;
    int size;
} MinHeap;

AdjList* createGraph() {
    AdjList* graph = (AdjList*)malloc(NODES * sizeof(AdjList));
    for (int i = 0; i < NODES; i++) {
        graph[i].head = NULL;
    }
    return graph;
}

//간선 추가
void addEdge(AdjList* graph, int src, int dest, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;
    newNode->weight = weight;
    newNode->next = graph[src].head;
    graph[src].head = newNode;

    newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = src;
    newNode->weight = weight;
    newNode->next = graph[dest].head;
    graph[dest].head = newNode;
}


MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->nodes = (MinHeapNode*)malloc(capacity * sizeof(MinHeapNode));
    heap->size = 0;
    return heap;
}


void insertMinHeap(MinHeap* heap, int vertex, int distance) {
    heap->nodes[heap->size].vertex = vertex;
    heap->nodes[heap->size].distance = distance;
    int i = heap->size++;
    while (i > 0 && heap->nodes[i].distance < heap->nodes[(i - 1) / 2].distance) {
        MinHeapNode temp = heap->nodes[i];
        heap->nodes[i] = heap->nodes[(i - 1) / 2];
        heap->nodes[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

// 최소 힙ㅇㅇ에서 최소값 추출
MinHeapNode extractMin(MinHeap* heap) {
    MinHeapNode root = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->size];

    int i = 0;
    while (i * 2 + 1 < heap->size) {
        int smallest = i;
        int left = i * 2 + 1;
        int right = i * 2 + 2;
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance)
            smallest = left;
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance)
            smallest = right;
        if (smallest != i) {
            MinHeapNode temp = heap->nodes[i];
            heap->nodes[i] = heap->nodes[smallest];
            heap->nodes[smallest] = temp;
            i = smallest;
        }
        else break;
    }
    return root;
}

// Dijkstra 알고리즘 (연결 리스트 + Min Heap 사용)
void dijkstra(AdjList* graph, int src) {
    int distance[NODES];
    int found[NODES] = { 0 };

    for (int i = 0; i < NODES; i++) distance[i] = INF;
    distance[src] = 0;

    MinHeap* heap = createMinHeap(NODES);
    insertMinHeap(heap, src, 0);

    while (heap->size) {
        MinHeapNode minNode = extractMin(heap);
        int u = minNode.vertex;

        if (found[u]) continue;
        found[u] = 1;

        Node* temp = graph[u].head;
        while (temp) {
            int v = temp->vertex;
            int weight = temp->weight;
            if (distance[u] != INF && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                insertMinHeap(heap, v, distance[v]);
            }
            temp = temp->next;
        }

        
        printf("Distance: ");
        for (int i = 0; i < NODES; i++) {
            if (distance[i] == INF) printf("* ");
            else printf("%d ", distance[i]);
        }
        printf("\nFound: ");
        for (int i = 0; i < NODES; i++) {
            printf("%d ", found[i]);
        }
        printf("\n\n");
    }

    printf("Found Order: ");
    for (int i = 0; i < NODES; i++) {
        if (found[i]) {
            printf("%d ", i + 1);
        }
    }
    printf("\n");

    free(heap->nodes);
    free(heap);
}

int main() {
    AdjList* graph = createGraph();

    
    addEdge(graph, 0, 1, 3);
    addEdge(graph, 0, 5, 11);
    addEdge(graph, 0, 6, 12);
    addEdge(graph, 1, 2, 8);
    addEdge(graph, 1, 3, 4);
    addEdge(graph, 1, 4, 1);
    addEdge(graph, 2, 3, 2);
    addEdge(graph, 2, 6, 7);
    addEdge(graph, 2, 7, 5);
    addEdge(graph, 3, 4, 10);
    addEdge(graph, 3, 8, 14);
    addEdge(graph, 4, 5, 9);
    addEdge(graph, 4, 9, 18);
    addEdge(graph, 6, 9, 22);
    addEdge(graph, 7, 9, 21);
    addEdge(graph, 8, 9, 10);

    dijkstra(graph, 0); // 노드 1에서 시작하도록

    return 0;
}

