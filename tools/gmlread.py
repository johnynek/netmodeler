#!/usr/bin/env python

class GmlParser(object):

  def __init__(self, file_obj):
    self.fileob = file_obj
    self.linebuf = ""

  def parse(self):
    """Read the entire file_obj and return a list of key, value pairs"""
    self._advance()
    return self._readArray(True)

  def _advance(self):
    self.linebuf = self.linebuf.lstrip()
    while not self.linebuf:
      self.linebuf = self.fileob.readline()
      if not self.linebuf:
        raise EOFError
      self.linebuf = self.linebuf.strip()
    #make sure linebuf is not empty
    assert self.linebuf

  def _readAtom(self):
    key = self.linebuf.split()[0]
    restidx = self.linebuf.find(key)
    self.linebuf = self.linebuf[restidx + len(key):]
    return key

  def _readVal(self):
    if self.linebuf[0] == '"':
      return self._readString()  
    elif self.linebuf[0] == "[":
      return self._readArray()
    else:
      return self._readAtom()

  def _readArray(self, lookforend=False):
    if not lookforend:
      #cut off the "[" character
      self.linebuf = self.linebuf[1:]
    res = []
    try:
      self._advance()
      while lookforend or self.linebuf[0] != "]":
        key = self._readAtom()
        self._advance()
        val = self._readVal()
        res.append( (key,val) )
        self._advance()
      #skip the "]" character
      self.linebuf = self.linebuf[1:]
    except Exception as x:
      pass
    return res
  
  def _readString(self):
    if self.linebuf[0] != '"':
      raise Exception("Not at string")
    self.linebuf = self.linebuf[1:]
    end = self.linebuf.find('"')
    while end == -1:
      rest = self.fileob.readline()
      if not rest:
        raise EOFError
      self.linebuf = self.linebuf + rest
      end = self.linebuf.find('"')
    res = self.linebuf[0:end]
    #skip the string terminator
    self.linebuf = self.linebuf[end+1:]
    return res

def GmlToNm(gmlfile, nmfile):
  """convert Gml to a very easy to parse format"""
  gp = GmlParser(gmlfile)
  gmldata = gp.parse()
  for (k,v) in gmldata:
    if k == "graph":
      for (gk, gv) in v:
        if gk == "node":
          node_data = dict(gv)
          nmfile.write("%s : \n" % node_data['id'])
        if gk == "edge":
          edge_data = dict(gv)
          nmfile.write("%s : %s\n" % (edge_data['source'], edge_data['target']))

if __name__ == "__main__":
  import sys
  if sys.argv[0] == "gml2nm":
    GmlToNm(open(sys.argv[1]), sys.stdout)
  else:
    gp = GmlParser(open(sys.argv[1]))
    print gp.parse()  
