from pdfminer.pdfinterp import PDFResourceManager, PDFPageInterpreter
from pdfminer.converter import TextConverter
from pdfminer.layout import LAParams, LTContainer, LTText, LTTextBox, LTImage
from pdfminer.pdfpage import PDFPage
from cStringIO import StringIO


class ColumnConverter(TextConverter):
    def __init__(self, rsrcmgr, outfp, codec='utf-8', pageno=1, laparams=None,
                 showpageno=False, imagewriter=None):
        TextConverter.__init__(self, rsrcmgr, outfp, codec=codec,
                               pageno=pageno, laparams=laparams,
                               showpageno=showpageno, imagewriter=imagewriter)
        return

    def receive_layout(self, ltpage):
        self.line_cache = ""
        def render(item):
            if isinstance(item, LTContainer):
                for child in item:
                    render(child)
            elif isinstance(item, LTText):
                # self.write_text(item.get_text())
                if item.get_text() == "\n":
                    if (self.line_cache.find("\n") > -1):
                        self.line_cache = self.line_cache[self.line_cache.find("\n")+1:]
                self.line_cache += item.get_text()
            if isinstance(item, LTTextBox):
                if (item.x0 > 50 and item.x0 < 60):
                    # self.write_text("("+str(item.x0)+","+str(item.y0)+"){"+self.line_cache[:-1]+"}"+'\n')
                    self.write_text(self.line_cache[:-1]+'\n')
            elif isinstance(item, LTImage):
                if self.imagewriter is not None:
                    self.imagewriter.export_image(item)
        if self.showpageno:
            self.write_text('Page %s\n' % ltpage.pageid)
        render(ltpage)
        # self.write_text('\f')
        return


def convert_pdf_to_txt(path, pagenr):
    rsrcmgr = PDFResourceManager()
    retstr = StringIO()
    codec = 'utf-8'
    laparams = LAParams()
    device = ColumnConverter(rsrcmgr, retstr, codec=codec, laparams=laparams)
    fp = file(path, 'rb')
    interpreter = PDFPageInterpreter(rsrcmgr, device)
    password = ""
    maxpages = 0
    caching = True
    pagenos=set()

    pages = PDFPage.get_pages(fp, pagenos, maxpages=maxpages, password=password,caching=caching, check_extractable=True)
    counter = 0
    for page in pages:
        if (counter == pagenr):
            interpreter.process_page(page)
            # layout = device.get_layout()
        counter += 1
    # print counter
    # page = PDFPage.get_pages(fp, pagenos, maxpages=maxpages, password=password,caching=caching, check_extractable=True)[pagenr]
    # interpreter.process_page(page)

    text = retstr.getvalue()
    print text

    fp.close()
    device.close()
    retstr.close()
    return text

filename = "input.pdf"
for page  in range(6, 94):
    result = convert_pdf_to_txt(filename, page);
