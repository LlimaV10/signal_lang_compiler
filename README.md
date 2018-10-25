# signal_lang_compiler
Это компилятор языка signal. Включает в себя лексический, синтаксический (машина Кнута) анализы и генератор кода.

Граматика языка выглядит очень примитивно:

	1. <signal-program> --> <program>
	2. <program> --> PROGRAM <procedure-identifier> ; <block>. |
		PROCEDURE <procedure-identifier> <parameters-list> ; <block> ;
	3. <block> --> <declarations> BEGIN <statements-list> END
	4. <declarations> --> <label-declarations>
	5. <label-declarations> --> LABEL <unsigned-integer> <labels-list>; | <empty>
	6. <labels-list> --> , <unsigned-integer> <labels-list> | <empty>
	7. <parameters-list> --> ( <declarations-list> ) | <empty>
	8. <declarations-list> --> <empty>
	9. <statements-list> --> <empty>
	10. <procedure-identifier> --> <identifier>
	11. <identifier> --> <letter><string>
	12. <string> --> <letter><string> | <digit><string> | <empty>
	13. <unsigned-integer> --> <digit><digits-string>
	14. <digits-string> --> <digit><digits-string> | <empty>
	15. <digit> --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
	16. <letter> --> A | B | C | D | ... | Z

Примеры программы на языке signal:

	PROCEDURE HELLO();
	LABEL 111, 32, 222;
	BEGIN
	END;

или

	PROGRAM HELLO;
	LABEL 111, 32, 222;
	BEGIN
	END.

Целью было понять принцип работы лексического и синтаксического анализаторов кода.

Вывод программы для первого примера:

Лексический анализатор:

	1   1   402   PROCEDURE
	1   11  1001  HELLO
	1   16  40    (
	1   17  41    )
	1   18  59    ;
	2   1   403   LABEL
	2   7   501   111
	2   10  44    ,
	2   12  502   32
	2   14  44    ,
	2   16  503   222
	2   19  59    ;
	3   1   404   BEGIN
	4   1   405   END
	4   4   59    ;

Синтаксический анализатор:

	<signal-program>
	|....<program>
	|....|....402 PROCEDURE
	|....|....<procedure-identifier>
	|....|....|....<identifier>
	|....|....|....|....1001 HELLO
	|....|....<parameters-list>
	|....|....|....40 (
	|....|....|....<declarations-list>
	|....|....|....|....<empty>
	|....|....|....41 )
	|....|....59 ;
	|....|....<block>
	|....|....|....<declarations>
	|....|....|....|....<label-declarations>
	|....|....|....|....|....403 LABEL
	|....|....|....|....|....<unsigned-integer>
	|....|....|....|....|....|....501 111
	|....|....|....|....|....<labels-list>
	|....|....|....|....|....|....44 ,
	|....|....|....|....|....|....<unsigned-integer>
	|....|....|....|....|....|....|....502 32
	|....|....|....|....|....|....<labels-list>
	|....|....|....|....|....|....|....44 ,
	|....|....|....|....|....|....|....<unsigned-integer>
	|....|....|....|....|....|....|....|....503 222
	|....|....|....|....|....|....|....<labels-list>
	|....|....|....|....|....|....|....|....<empty>
	|....|....|....|....|....59 ;
	|....|....|....404 BEGIN
	|....|....|....<statements-list>
	|....|....|....|....<empty>
	|....|....|....405 END
	|....|....59 ;

Сгенерированный ассемблерный код:

	PROCEDURE_HELLO PROC
	   ?111:
	   ?32:
	   ?222:
	PROCEDURE_HELLO ENDP

Далее примеры ошибок

Лексическая ошибка:

	PROCEDURE 1HELLO();
	LABEL 111, 32, 222;
	BEGIN
	END;
	Lexer: Error (line 1, column 11): Undefined Identifier '1HELLO'

Синтаксическая ошибка:

	PROCEDURE HELLO();
	LABEL 111, 32 222;
	BEGIN
	END;
	Parser: Error (line 2, column 15): ','  or  ';' expected but '222' found

Ошибка при генерации кода:

	PROGRAM HELLO;
	LABEL 111, 32, 32, 666;
	BEGIN
	END.
	Code Generator: Error (line 2, column 16): Label '32' already taken
