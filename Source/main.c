#include "Code_generator.h"

int main()
{
	int				fd;
	t_leks_list		*leks;
	//char			**table;
	t_parser_tree	*tree;
	t_code			*code;

	leks = lekser("code.signal");
	output_leks(leks, 0, "out.txt");
	if (leks != 0)
		leks = get_reversed_list(leks);
	if (leks_error(leks) == 0)
	{
		tree = get_tree(leks);
		print_tree(tree, 0, "out.txt");
		if (!((tree->next == 0) && (tree->in == 0)))
		{
			code = code_generator(leks, "");
			out_code(code, 0, "out.txt");
		}
	}
	/*else
		output_leks(leks, 1, "out.txt");*/
	/*fd = _open("out2.txt", O_WRONLY | O_CREAT);
	check_identifier(0, 0, &table);
	if (table != 0)
		out_table(table, 1001, 0);
	check_number(0, 0, &table);
	if (table != 0)
		out_table(table, 501, 0);
	_close(fd);*/
	system("PAUSE");
	return (0);
}