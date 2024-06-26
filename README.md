# Simple Bash Utils

Разработка утилит Bash по работе с текстом: cat, grep.

## cat

Cat - одна из наиболее часто используемых команд в Unix-подобных операционных системах. Команда имеет три взаимосвязанные функции в отношении текстовых файлов: отображение, объединение их копий и создание новых.

### cat Использование

`cat [OPTION] [FILE]...`

### cat Опции

| № | Опции | Описание |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | нумерует только непустые строки |
| 2 | -e предполагает и -v (GNU only: -E то же самое, но без применения -v) | также отображает символы конца строки как $  |
| 3 | -n (GNU: --number) | нумерует все выходные строки |
| 4 | -s (GNU: --squeeze-blank) | сжимает несколько смежных пустых строк |
| 5 | -t предполагает и -v (GNU: -T то же самое, но без применения -v) | также отображает табы как ^I |

## grep

Grep — это утилита командной строки, используемая в операционных системах UNIX и Linux для поиска текстовых строк в файлах или потоках данных. Название grep происходит от фразы "Global Regular Expression Print" (Глобальный вывод регулярного выражения).

### grep Использование

`grep [options] template [file_name]`

### grep Опции

| № | Опции | Описание |
| ------ | ------ | ------ |
| 1 | -e | Шаблон |
| 2 | -i | Игнорирует различия регистра.  |
| 3 | -v | Инвертирует смысл поиска соответствий. |
| 4 | -c | Выводит только количество совпадающих строк. |
| 5 | -l | Выводит только совпадающие файлы.  |
| 6 | -n | Предваряет каждую строку вывода номером строки из файла ввода. |
| 7 | -h | Выводит совпадающие строки, не предваряя их именами файлов. |
| 8 | -s | Подавляет сообщения об ошибках о несуществующих или нечитаемых файлах. |
| 9 | -f file | Получает регулярные выражения из файла. |
| 10 | -o | Печатает только совпадающие (непустые) части совпавшей строки. |


## Chapter III

- Программы разработаны на языке Си стандарта C11 с использованием компилятора gcc.

- При написании кода придерживались Google Style.

- Программы представляют собой исполняемый файл с аргументами командной строки.

- Сборка программ настроена с помощью Makefile с целями: "s21_cat" и "s21_grep".

- Программа покрыта интеграционными тестами для всех вариантов флагов и входных значений, основанными на сравнении с поведением реальных утилит Bash.

## Part 1. Работа с утилитой cat

Утилита cat разработана соответствующим образом:

Утилита поддерживает все флаги (-b, -e, -n, -s, -t, -v), включая их парные комбинации (например, -be, -tv).

## Part 2. Работа с утилитой grep

Утилита grep разработана соответствующим образом:

- Утилита поддерживает все флаги (-e, -i, -v, -c, -l, -n, -s, -f, -o), включая их парные комбинации (например, -iv, -in и т.д.).
Для работы с регулярными выражениями используются только библиотека Regex.

