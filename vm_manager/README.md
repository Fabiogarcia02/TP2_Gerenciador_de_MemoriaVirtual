# Trabalho Prático 2 — Gerenciador de Memória Virtual

Este é o projeto-base para implementação de um simulador de gerência de memória virtual.

## Estrutura

```text
vm_manager/
├── Makefile
├── README.md
├── include/
│   ├── config.h
│   ├── tlb.h
│   ├── page_table.h
│   ├── memory.h
│   └── statistics.h
├── src/
│   ├── main.c
│   ├── tlb.c
│   ├── page_table.c
│   ├── memory.c
│   └── statistics.c
├── data/
│   └── generate_data.py
└── report/
```

## Gerar arquivos comuns de entrada

Entre no diretório `data` e execute:

```bash
python3 generate_data.py
```

Isso criará:

- `BACKING_STORE.bin`
- `addresses_random.txt`
- `addresses_location.txt`

## Compilação

Na raiz do projeto:

```bash
make
```

## Execução

```bash
./vm < data/addresses_random.txt
```

ou

```bash
./vm < data/addresses_location.txt
```

