from random import randint, seed

SEED = 42
NUM_ADDRESSES = 10000
VIRTUAL_MEMORY_SIZE = 65536
PAGE_SIZE = 256
NUM_PAGES = 256

seed(SEED)

def generate_backing_store():
    # Gera um BACKING_STORE.bin determinístico de 65.536 bytes.
    # O valor de cada byte será i % 256.
    # Como char em C pode ser sinalizado, valores acima de 127
    # poderão aparecer como negativos na saída do programa.
    with open("BACKING_STORE.bin", "wb") as f:
        for i in range(VIRTUAL_MEMORY_SIZE):
            f.write(bytes([i % 256]))

def generate_random_addresses():
    # Gera 10.000 endereços virtuais aleatórios no intervalo [0, 65535].
    with open("addresses_random.txt", "w", encoding="utf-8") as f:
        for _ in range(NUM_ADDRESSES):
            address = randint(0, VIRTUAL_MEMORY_SIZE - 1)
            f.write(f"{address}\n")

def generate_locality_addresses():
    # Gera 10.000 endereços com localidade de referência.
    # A cada bloco de acessos, algumas páginas próximas são reutilizadas.
    with open("addresses_location.txt", "w", encoding="utf-8") as f:
        current_page = randint(0, NUM_PAGES - 1)

        for i in range(NUM_ADDRESSES):
            if i % 200 == 0:
                current_page = randint(0, NUM_PAGES - 1)

            # 85% dos acessos ficam em páginas próximas.
            if randint(1, 100) <= 85:
                delta = randint(-2, 2)
                page = max(0, min(NUM_PAGES - 1, current_page + delta))
            else:
                page = randint(0, NUM_PAGES - 1)

            offset = randint(0, PAGE_SIZE - 1)
            address = page * PAGE_SIZE + offset
            f.write(f"{address}\n")

if __name__ == "__main__":
    generate_backing_store()
    generate_random_addresses()
    generate_locality_addresses()

    print("Arquivos gerados com sucesso:")
    print("- BACKING_STORE.bin")
    print("- addresses_random.txt")
    print("- addresses_location.txt")
    print(f"Seed utilizada: {SEED}")
