#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>

#define in_ns "https://example.org/input"

void fail(char *file, int line);

#define assert(check)                                   \
    do {                                                \
        if(!(check)) {                                  \
            fail(__FILE__, __LINE__);                   \
        }                                               \
    } while(0)                                  

void print_doc(char *name, xmlDoc *doc);
xsltStylesheet *read_style(char *path);
xmlDoc *read_doc(char *path);

int main(){
    xmlDoc *in_mem_doc = xmlNewDoc(BAD_CAST "1.0");
    assert(in_mem_doc);
    xmlNode *root = xmlNewNode(NULL, BAD_CAST "input");
    assert(root);
    xmlDocSetRootElement(in_mem_doc, root);
    xmlNs *ns = xmlNewNs(root, BAD_CAST in_ns, NULL);
    assert(ns);
    // the following xmlSetNs is mandatory so that the in memory
    // document has the namespace applied. interestingly the written
    // xml document's input elemens will have the namespace applied
    // even if we don't call xmlSetNs.
    xmlSetNs(root, ns);
    print_doc("in_mem_doc", in_mem_doc);
    xmlDoc *in_fs_doc = read_doc("input.xml");
    print_doc("in_fs_doc", in_fs_doc);
    xsltStylesheet *style = read_style("style.xslt");
    assert(style);
    const char *xslt_params[] = {
        NULL
    };
    xmlDoc *out_mem_doc = xsltApplyStylesheet(style, in_mem_doc, xslt_params);
    assert(out_mem_doc);
    print_doc("out_mem_doc", out_mem_doc);
    xmlFreeDoc(out_mem_doc);
    xmlDoc *out_fs_doc = xsltApplyStylesheet(style, in_fs_doc, xslt_params);
    assert(out_fs_doc);
    print_doc("out_fs_doc", out_fs_doc);
    xsltFreeStylesheet(style);
    xmlFreeDoc(in_mem_doc);
    return 0;
}

void fail(char *file, int line){
    printf("%s:%d: failed\n", file, line);
    exit(1);
}

void print(char *s, size_t len);

void print_doc(char *name, xmlDoc *doc){
    print(name, strlen(name));
    print("\n", 1);
    char *doc_str = NULL;
    int doc_size;
    xmlDocDumpMemoryEnc(doc, (xmlChar**)&doc_str, &doc_size, "UTF-8");
    assert(doc_str);
    print(doc_str, doc_size);
    print("\n", 1);
    xmlFree(doc_str);
}

xsltStylesheet *read_style(char *path){
    xmlDoc *doc = read_doc(path);
    assert(doc);
    xsltStylesheet *style = xsltParseStylesheetDoc(doc);
    assert(style);
    return style;
}

xmlDoc *read_doc(char *path){
    int fd = open(path, O_RDONLY);
    assert(fd >= 0);
    char buf[4096];
    char *end = &buf[sizeof(buf)];
    char *b = buf;
    while(1){
        size_t remaining = end - b;
        assert(remaining > 0);
        ssize_t bytes_read = read(fd, b, remaining);
        assert(bytes_read >= 0);
        if(bytes_read == 0){
            break;
        }
        b += bytes_read;
    }
    close(fd);
    xmlDoc *doc = xmlReadMemory(buf, b - buf, NULL, "UTF-8", 0);
    assert(doc);
    return doc;
}

void print(char *s, size_t len){
    ssize_t written = write(1, s, len);
    assert(written == len);
}
