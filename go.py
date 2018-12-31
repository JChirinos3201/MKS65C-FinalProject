import re

#left off at 1063
def go_black(start=0):
    at = 0
    with open('cah.tsv') as f, open('black.csv', 'a+') as b:
        for line in f:
            if at >= start:
                print('at: {}'.format(at))
                card = line.split('\t')[0].replace('"', '').capitalize()
                if 'Insert Name' not in card and 'insert name' not in card and 'Insert name' not in card and len(re.findall('_+', card)) == 1:
                    print('black card: {}'.format(card))
                    response = input()
                    if str(response) == 'y':
                        b.write('{}\n'.format(card))
                    if str(response) == 'done':
                        break
            at += 1

def go_white(start=0):

    with open('cah.csv') as f:
        for line in f:
            pass

