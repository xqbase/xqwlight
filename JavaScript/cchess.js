"use strict";

function CHR(n) {
  return String.fromCharCode(n);
}

function ASC(c) {
  return c.charCodeAt(0);
}

function move2Iccs(mv) {
  var sqSrc = SRC(mv);
  var sqDst = DST(mv);
  return CHR(ASC("A") + FILE_X(sqSrc) - FILE_LEFT) +
      CHR(ASC("9") - RANK_Y(sqSrc) + RANK_TOP) + "-" +
      CHR(ASC("A") + FILE_X(sqDst) - FILE_LEFT) +
      CHR(ASC("9") - RANK_Y(sqDst) + RANK_TOP);
}