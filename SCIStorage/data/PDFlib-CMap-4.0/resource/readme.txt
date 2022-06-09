-----------------------------
CJK CMaps for PDFlib Products
-----------------------------

PDFlib-CMap-4.0

This package contains CMaps for the following Adobe character collections:

- Adobe-Japan1-7 for Japanese
- Adobe-CNS1-7 for traditional Chinese
- Adobe-GB1-5 for simplified Chinese
- Adobe-Korea1-1 for Korean

The CMaps are required in the following situations:

- Creating text output with PDFlib and CJK legacy encodings. The CMaps are
  not required in Unicode workflows.

- Extracting text from certain PDF documents with PDFlib TET. The CMaps are
  only required for legacy PDF documents which contain CJK text in legacy
  encodings by using unembedded predefined CMaps. TET 5.2 and above report
  an error if a CMap required for text extraction is not available due to
  a configuration problem.

The CMap files should be installed as follows:

- You can place the CMap files in any convenient directory
  and must manually configure CMap access by setting the searchpath at
  runtime:

  p.set_option("searchpath=/path/to/resource/cmap")

- As an alternative method for configuring access to the CJK CMap files you
  can set the PDFLIBRESOURCEFILE environment variable to point to a UPR
  configuration file which contains a suitable SearchPath definition.

Refer to the PDFlib documentation for more information on CJK text handling.


PDFlib GmbH
Franziska-Bilek-Weg 9
80339 Munich, Germany
tel. +49 89 452 33 84-0
info@pdflib.com
www.pdflib.com
