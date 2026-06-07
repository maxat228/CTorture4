# Лабораторная работа №4. B-дерево

## Описание
Реализация B-дерева порядка 2 (2-3-4 дерево) с операциями вставки, удаления, поиска и печати. Обработка команд из текстового файла.

## Вариант
N=16, (16%3)+1 = 2 — B-дерево порядка 2.

## Структура проекта
```
CTorture4/
├── src/
│ ├── functions/
│ │ ├── functions.h
│ │ └── functions.c
│ └── structures/
│ └── btree.h
├── tests.cpp # GTest
├── cli.c # CLI
├── main.c
├── input.txt # Входные команды
├── CMakeLists.txt
├── report.md
├── README.md
└── .gitignore
```

## Реализованные операции
- Вставка с расщеплением
- Удаление с заимствованием и слиянием
- Поиск по ключу
- Печать дерева
- Файловый ввод/вывод

## Сборка
```bash
cmake -B build
cmake --build build
./build/CTorture4
./build/tests
./build/cli
