from subprocess import call

filename = "list_of_words.txt"
f = open(filename, "r")
lines = f.read().split("\n")
# print lines
for i in lines:
    call(["wget", "https://images.search.yahoo.com/search/images?p=" + i])
