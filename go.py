import re

# left off at 2000
def go_black(start=0):
    at = 0
    with open('cah.tsv') as f, open('black.csv', 'a+') as b:
        for line in f:
            if at >= start:
                print('at: {}'.format(at))
                card = line.split('\t')[0].replace('"', '').replace('“', '').replace('”', '').capitalize()
                if 'Insert Name' not in card and 'insert name' not in card and 'Insert name' not in card and len(re.findall('_+', card)) == 1:
                    print('black card: {}'.format(card))
                    response = input()
                    if str(response) == 'y':
                        b.write('{}\n'.format(card))
                    if str(response) == 'done':
                        break
            at += 1

# left off at 373
def go_white(start=0):
    at = 0
    with open('cah.tsv') as f, open('white.csv', 'a+') as w:
        for line in f:
            if at >= start:
                print('at: {}'.format(at))
                card = line.split('\t')[6].replace('"', '').replace('“', '"').replace('”', '"')
                if card[0] == '"' and card[-1] == '"':
                    card = card[1:-1]
                if card[-1] == '.':
                    card = card[:-1]
                card = card.capitalize()
                print('white card: {}'.format(card))
                response = input()
                if str(response) == 'y':
                    w.write('{}\n'.format(card))
                if str(response) == 'done':
                    break
            at += 1
