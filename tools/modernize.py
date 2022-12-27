import fileinput


def fixdbs(line):
    if not line.lstrip().startswith("db"):
        return line
    indent, args = line.split("db", 1)
    instr = False;
    comment = False;
    fixed = indent + "db"
    for c in args:
        if comment:
            fixed += c
            continue
        if c == "'":
            fixed += '"'
            instr = not instr
            continue
        if c == '"' and instr:
            fixed += '\\"'
            continue
        if c == ";" and not instr:
            comment= True
        fixed += c
    return fixed

for line in fileinput.input():
    if line.strip() == 'end':
        break
    if line[0].isalpha():
        parts = line.split(None, 1)
        if len(parts) == 1:
            line = f"{line.rstrip()}:\n"
        elif len(parts) > 1:
            label, rest = parts 
            rest = fixdbs(rest)
            line = f"{label}:\t{rest}"
    else:
        line = fixdbs(line)
    line = line.replace("'\\'", "'\\\\'")
    line = line.replace("''''", "'\\''")
    print(line, end='')
