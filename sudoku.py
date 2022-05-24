import random

# import sys


BOX_ROW_LENGTH = 3
BOX_COL_LENGTH = 3
FULL_LENGTH = BOX_ROW_LENGTH * BOX_COL_LENGTH
FILE_PATH = "sudoku.txt"


def get_possiblity(table: list, row: int, column: int) -> set:
    return (
        {i for i in range(1, FULL_LENGTH + 1)}
        - {table[row][i] for i in range(FULL_LENGTH) if i != column}
        - {table[i][column] for i in range(FULL_LENGTH) if i != row}
        - {
            table[i][j]
            for i in range(
                row - row % BOX_ROW_LENGTH,
                row - row % BOX_ROW_LENGTH + BOX_ROW_LENGTH,
            )
            if i != row
            for j in range(
                column - column % BOX_COL_LENGTH,
                column - column % BOX_COL_LENGTH + BOX_COL_LENGTH,
            )
            if j != column
        }
    )


def find_answer(table: list, row: int = 0, column: int = 0) -> list:

    if row == FULL_LENGTH:
        return [[table_row[:] for table_row in table]]

    answers = []

    if table[row][column] == 0:

        for possible_num in get_possiblity(table, row, column):
            table[row][column] = possible_num

            answers.extend(
                find_answer(
                    table,
                    row + (column + 1) // FULL_LENGTH,
                    (column + 1) % FULL_LENGTH,
                )
            )

        table[row][column] = 0

    elif table[row][column] in get_possiblity(table, row, column):

        answers.extend(
            find_answer(
                table,
                row + (column + 1) // FULL_LENGTH,
                (column + 1) % FULL_LENGTH,
            )
        )

    return answers


def make_table() -> list:
    (*l,) = range(1, FULL_LENGTH + 1)
    random.shuffle(l)
    return [
        l[i + j * BOX_COL_LENGTH :] + l[: i + j * BOX_COL_LENGTH]
        for i in range(BOX_COL_LENGTH)
        for j in range(BOX_ROW_LENGTH)
    ]


def transpose(table: list) -> list:
    i_range = random.choice(
        [range(FULL_LENGTH), range(FULL_LENGTH - 1, -1, -1)]
    )
    j_range = random.choice(
        [range(FULL_LENGTH), range(FULL_LENGTH - 1, -1, -1)]
    )

    is_xy_convert = random.choice([False, BOX_ROW_LENGTH == BOX_COL_LENGTH])

    return [
        [table[j][i] if is_xy_convert else table[i][j] for j in j_range]
        for i in i_range
    ]


def change_row(table: list) -> list:
    shuffle_line = [
        [BOX_ROW_LENGTH * i + j for j in range(BOX_ROW_LENGTH)]
        for i in range(BOX_COL_LENGTH)
    ]

    for i in range(BOX_COL_LENGTH):
        random.shuffle(shuffle_line[i])
    random.shuffle(shuffle_line)

    return [table[j] for i in shuffle_line for j in i]


def change_col(table: list) -> list:
    shuffle_line = [
        [BOX_COL_LENGTH * i + j for j in range(BOX_COL_LENGTH)]
        for i in range(BOX_ROW_LENGTH)
    ]

    for i in range(BOX_ROW_LENGTH):
        random.shuffle(shuffle_line[i])
    random.shuffle(shuffle_line)

    return [
        [table[row][j] for i in shuffle_line for j in i]
        for row in range(FULL_LENGTH)
    ]


def change_num(table: list) -> list:
    (*shuffle_num,) = range(1, FULL_LENGTH + 1)
    random.shuffle(shuffle_num)
    return [[shuffle_num[col - 1] for col in row] for row in table]


def get_blank(table: list) -> list:
    non_zero = [
        (i, j)
        for i in range(FULL_LENGTH)
        for j in range(FULL_LENGTH)
        if table[i][j] != 0
    ]

    random.shuffle(non_zero)

    zero_count = random.randint(
        FULL_LENGTH ** 2 // 2, FULL_LENGTH ** 2 * 4 // 5
    )

    for non_zero_i, non_zero_j in non_zero:
        temp = table[non_zero_i][non_zero_j]
        table[non_zero_i][non_zero_j] = 0
        if len(find_answer(table, 0, 0)) > 1:
            table[non_zero_i][non_zero_j] = temp
        else:
            zero_count -= 1
            if zero_count == 0:
                break

    return table


def make_sudoku():
    table = make_table()
    transforms = [transpose, change_row, change_col]
    for _ in [0] * 1000:
        table = random.choice(transforms)(table)

    table = change_num(table)
    table = get_blank(table)

    with open(FILE_PATH, "w") as f:
        for row in table:
            f.write(" ".join(map(str, row)))
            f.write("\n")


def solve_sudoku():
    table = []

    with open(FILE_PATH, "r") as f:
        for line_idx, line in enumerate(f.readlines()):
            table.append(list(map(int, line.split())))

    for answer in find_answer(table):
        print(*answer, sep="\n", end="\n\n")


def main():
    make_sudoku()
    solve_sudoku()


if __name__ == "__main__":
    main()
