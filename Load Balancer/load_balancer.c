// Copyright 2021 <Manolache Marius-Alexandru>

#include <stdlib.h>
#include <string.h>
#include "load_balancer.h"

struct load_balancer
{
    linked_list_t *hashring;
    server_memory *server_array[NSERV];
};

unsigned int hash_function_servers(void *a)
{
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

unsigned int hash_function_key(void *a)
{
    unsigned char *puchar_a = (unsigned char *)a;
    unsigned int hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}

load_balancer *init_load_balancer()
{
    load_balancer *main = calloc(1, sizeof(load_balancer));
    DIE(!main, "A bubuit callocu la init_load_balancer");

    for (int i = 0; i < NSERV; i++)
        main->server_array[i] = NULL;

    main->hashring = ll_create(sizeof(server_info));

    return main;
}

void loader_store(load_balancer *main, char *key, char *value, int *server_id)
{
    // Trecem prin toate hash urile serverelor si il gasim pe primul mai mare
    // decat hash-ul cheii

    ll_node_t *runner = main->hashring->head;

    server_info *server;

    unsigned int hash = hash_function_key(key);

    while (runner) {
        server = ((server_info *)(runner->data));

        if (server->server_hash > hash) {
            server_store(main->server_array[server->server_id], key, value);
            *server_id = server->server_id;
            return;
        }

        runner = runner->next;
    }

    // Cheia are cel mai mare hash, deci ajunge in primul server

    runner = main->hashring->head;
    server = ((server_info *)(runner->data));

    server_store(main->server_array[server->server_id], key, value);

    *server_id = server->server_id;
}

char *loader_retrieve(load_balancer *main, char *key, int *server_id)
{
    // Trecem prin toate hash urile serverelor si il gasim pe primul mai mare
    // decat hash-ul cheii

    ll_node_t *runner = main->hashring->head;

    server_info *server;

    unsigned int hash = hash_function_key(key);

    while (runner) {
        server = ((server_info *)(runner->data));

        if (server->server_hash > hash) {
            *server_id = server->server_id;
            return server_retrieve(main->server_array[server->server_id], key);
        }

        runner = runner->next;
    }

    // Hash-ul este mai mare decat hash-ul oricarui server
    // Deci verificam in primul server

    runner = main->hashring->head;
    server = ((server_info *)(runner->data));
    *server_id = server->server_id;

    return server_retrieve(main->server_array[server->server_id], key);
}

void transition_key(load_balancer *main, char *key, char *value,
	int server_id_1, int server_id_2)
{
    server_store(main->server_array[server_id_1], key, value);
    server_remove(main->server_array[server_id_2], key);
}

void add_rearrange_data(load_balancer *main, int server_id_1, int server_id_2,
    unsigned int server_hash_1, unsigned int prev_hash, int last)
{
    // Parcurgem ht-ul din server 2 si vedem
    // care elemente au hash-ul mai mic decat cel al lui server1

    hashtable_t *ht = main->server_array[server_id_2]->ht;

    if (!ht->size)
        return;

    char *key;
    char *value;

    ll_node_t *runner;
    ll_node_t *head = main->hashring->head;
    ll_node_t *tail = ll_get_nth_node(main->hashring, main->hashring->size);

    int n = ht->hmax;

    // Trei cazuri, la inceput, la sfarsit sau in interiorul hashringului
    // Difera conditia din if-ul din while

    if (!last && server_id_2 == ((server_info *)(head->data))->server_id &&
    	prev_hash > server_hash_1) {
        for (int i = 0; i < n; i++) {
            runner = ht->buckets[i]->head;
            while (runner) {
                key = (char *)((struct info *)(runner->data))->key;
                if (hash_function_key(key) < server_hash_1 ||
                    hash_function_key(key) >
                    ((server_info *)(tail->data))->server_hash) {
                    value = (char *)((struct info *)(runner->data))->value;
                    runner = runner->next;
                    transition_key(main, key, value, server_id_1, server_id_2);
                } else {
                    runner = runner->next;
                }
            }
        }
	} else if (last) {
        for (int i = 0; i < n; i++) {
            runner = ht->buckets[i]->head;
            while (runner) {
                key = (char *)((struct info *)(runner->data))->key;
                if (hash_function_key(key) < server_hash_1 &&
                    hash_function_key(key) >
                    ((server_info *)(tail->data))->server_hash) {
                    value = (char *)((struct info *)(runner->data))->value;
                    runner = runner->next;
                    transition_key(main, key, value, server_id_1, server_id_2);
                } else {
                    runner = runner->next;
                }
            }
        }
    } else {
        for (int i = 0; i < n; i++) {
            runner = ht->buckets[i]->head;
            while (runner) {
                key = (char *)((struct info *)(runner->data))->key;
                if (hash_function_key(key) < server_hash_1 &&
                	hash_function_key(key) > prev_hash) {
                    value = (char *)((struct info *)(runner->data))->value;
                    runner = runner->next;
                    transition_key(main, key, value, server_id_1, server_id_2);
                } else {
                    runner = runner->next;
                }
            }
        }
    }
}

void remove_rearrange_data(load_balancer *main, int server_id_1,
	int server_id_2)
{
    hashtable_t *ht = main->server_array[server_id_1]->ht;

    if (!ht->size)
        return;

    char *key;
    char *value;

    ll_node_t *runner;

    int n = ht->hmax;

    for (int i = 0; i < n; i++) {
        runner = ht->buckets[i]->head;
        while (runner) {
            key = (char *)((struct info *)(runner->data))->key;
            value = (char *)((struct info *)(runner->data))->value;

            // Facem tranzitia in urmatorul server
            // Nu mai trebuie apelat server_remove pentru ca oricum isi ia free

            server_store(main->server_array[server_id_2], key, value);

            runner = runner->next;
        }
    }
}

void sort_server_hashes(server_info servers[3], int server_id)
{
    unsigned int hashes[3];
    int copy_server;

    for (int i = 0; i < 3; i++) {
        copy_server = i * 100000 + server_id;
        hashes[i] = hash_function_servers(&copy_server);
    }

    int replicas_order[3];
    for (int i = 0; i < 3; i++)
        replicas_order[i] = i;

    unsigned int copy;

    // Practic ordonez replicile server-ului in functie de hashurile lor

    for (int i = 0; i < 2; i++)
        for (int j = i + 1; j < 3; j++)
            if (hashes[i] > hashes[j]) {
                copy = replicas_order[i];
                replicas_order[i] = replicas_order[j];
                replicas_order[j] = copy;

                copy = hashes[i];
                hashes[i] = hashes[j];
                hashes[j] = copy;
            }

    // Aici populam vectorul de servere in ordine

    for (int i = 0; i < 3; i++) {
        servers[i].server_id = server_id;
        servers[i].server_label = replicas_order[i];
        copy_server = replicas_order[i] * 100000 + server_id;
        servers[i].server_hash = hash_function_servers(&copy_server);
    }
}

void remove_from_hashring(load_balancer *main, int server_id)
{
    // Pentru un cod mai curat si compact sortam hashurile
    // tuturor celor 3 replici
    // This will come in handy later

    server_info servers[3];

    sort_server_hashes(servers, server_id);

    int count = 0;
    int replicas_count = 0;

    ll_node_t *runner = main->hashring->head;
    ll_node_t *thrash;

    // Fiind in ordine replicile serverelor
    // le scoatem exact ca in array

    while (runner && replicas_count < 3) {
        if (((server_info *)(runner->data))->server_hash ==
        	servers[replicas_count].server_hash) {
            // Daca este ultimul server, load ul va fi preluat de primul

            if (runner->next)
                remove_rearrange_data(main, server_id,
                	((server_info *)(runner->next->data))->server_id);
            else
                remove_rearrange_data(main, server_id,
                	((server_info *)(main->hashring->head->data))->server_id);

            runner = runner->next;

            thrash = ll_remove_nth_node(main->hashring, count);

            free(thrash->data);
            free(thrash);

            thrash = NULL;

            replicas_count++;
        } else {
            count++;
            runner = runner->next;
        }
    }
}

void add_to_hashring(load_balancer *main, int server_id)
{
    // Pentru un cod mai curat si compact sortam hashurile tuturor
    // celor 3 replici
    // This will come in handy later

    server_info servers[3];

    sort_server_hashes(servers, server_id);

    // Daca lista e goala adaugam replicile ordonate

    if (!main->hashring->size) {
        for (int i = 0; i < 3; i++)
            ll_add_nth_node(main->hashring, i, &servers[i]);
        return;
    }

    // Minim 3 noduri in lista

    int count = 0;
    int replicas_count = 0;

    ll_node_t *runner = main->hashring->head;
    ll_node_t *prev;
    ll_node_t *tail = ll_get_nth_node(main->hashring, main->hashring->size);

    unsigned int prev_hash = 0;

    // Fiind in ordine replicile serverelor
    // le adaugam exact ca in array

    while (runner && replicas_count < 3) {
        if (((server_info *)(runner->data))->server_hash >
        	servers[replicas_count].server_hash) {
            // Hashringul fiind ordonat e clar ca load-ul va fi preluat
            // de la urmatorul server clock-wise

            if (count - 1 < 0)
                prev = tail;
            else
                prev = ll_get_nth_node(main->hashring, count - 1);

            prev_hash = ((server_info *)(prev->data))->server_hash;

            add_rearrange_data(main, server_id,
                               ((server_info *)(runner->data))->server_id,
                               servers[replicas_count].server_hash, prev_hash,
                               0);

            ll_add_nth_node(main->hashring, count, &servers[replicas_count]);

            replicas_count++;
        } else {
            runner = runner->next;
        }

        count++;
    }

    // A ajuns la finalul hashringului dar inca mai are de adaugat servere

    runner = main->hashring->head;

    for (int i = replicas_count; i < 3; i++) {
        add_rearrange_data(main, server_id,
                           ((server_info *)(runner->data))->server_id,
                           servers[i].server_hash, 0, 1);

        ll_add_nth_node(main->hashring, main->hashring->size, &servers[i]);
    }
}

void loader_add_server(load_balancer *main, int server_id)
{
    main->server_array[server_id] = init_server_memory();

    add_to_hashring(main, server_id);
}

void loader_remove_server(load_balancer *main, int server_id)
{
    remove_from_hashring(main, server_id);

    free_server_memory(main->server_array[server_id]);

    main->server_array[server_id] = NULL;
}

void free_load_balancer(load_balancer *main)
{
    for (int i = 0; i < NSERV; i++)
        if (main->server_array[i])
            free_server_memory(main->server_array[i]);

    ll_free(&(main->hashring));

    free(main);
}
