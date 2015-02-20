#ifndef __DBGMACROS_H__
#define __DBGMACROS_H__

bool _dbgprint = false;
void enableDebugPrint() {
  _dbgprint=true;
}

#define dbgcout if (dbgprint) cout

#endif
