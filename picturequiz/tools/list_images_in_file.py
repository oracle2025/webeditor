from bs4 import BeautifulSoup
from subprocess import call
import urllib


def unique(it):
    s = set()
    for el in it:
        if el not in s:
            s.add(el)
            yield el


def download_images_for(what):
    filename = "images?p=" + urllib.quote(what)
    print filename
    # filename = "images_monkey_tidy.html"
    f = open(filename, "r")
    html_doc = f.read()
    soup = BeautifulSoup(html_doc, 'html.parser')
    results = soup.find(id="results")
    images = []
    for link in soup.find_all('img'):
        images.append(link.get('src'))
    if len(images) > 0:
        images = list(unique(images))
        counter = 0
        prefix = what + "-"
        extension = ".jpg"
        for i in images[0:10]:
            if i:
                print i
                filename = prefix + str(counter) + extension
                print filename
                # call(["wget", i + " --output-document=images/"+filename])
                # call(["echo ", i, "--output-document=images/"+filename])
                call(["wget", i, "--output-document=images/"+filename])
                counter += 1

filename = "list_of_words.txt"
f = open(filename, "r")
for i in f.read().split("\n"):
    download_images_for(i)
