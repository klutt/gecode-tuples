// branching
// branch function types
namespace MPG {
  typedef bool   (*IntPairBranchFilter)(const Space& home, 
					IntPairVar x, int i);
  typedef void   (*IntPairVarValPrint) (const Space &home, 
					const BrancherHandle& bh,
					unsigned int a,
					IntPairVar x, int i, const int& n,
					std::ostream& o);
  typedef double (*IntPairBranchMerit) (const Space& home, 
					IntPairVar x, int i); 
  typedef int    (*IntPairBranchVal)   (const Space& home, 
					IntPairVar x, int i);
  typedef void   (*IntPairBranchCommit)(Space& home, unsigned int a,
					IntPairVar x, int i, int n); 
}

    
// branch traits
namespace Gecode {
  template<>
  class BranchTraits<MPG::IntPairVar> {
  public:
    typedef MPG::IntPairBranchFilter Filter;
    typedef MPG::IntPairBranchMerit Merit;
    typedef MPG::IntPairBranchVal Val;
    typedef MPG::IntPair::Pair ValType;
    typedef MPG::IntPairBranchCommit Commit;
  };
}
/*
// variable AFC
namespace MPG {
  class IntAFC : public AFC {
  public:
    IntAFC(void);
    IntAFC(const IntAFC& a);
    IntAFC& operator =(const IntAFC& a);
    IntAFC(Home home, const IntVarArgs& x, double d=1.0);
    void init(Home home, const IntVarArgs& x, double d=1.0);
  };
  inline
  IntAFC::IntAFC(void) {}
  inline
  IntAFC::IntAFC(const IntAFC& a)
    : AFC(a) {}
  inline IntAFC&
  IntAFC::operator =(const IntAFC& a) {
    return static_cast<IntAFC&>(AFC::operator =(a));
  }
  inline
  IntAFC::IntAFC(Home home, const IntVarArgs& x, double d) {
    AFC::init(home,x,d);
  }
  inline void
  IntAFC::init(Home home, const IntVarArgs& x, double d) {
    AFC::init(home,x,d);
  }
}

// variable activity
namespace MPG {
  class IntActivity : public Activity {
  public:
    IntActivity(void);
    IntActivity(const IntActivity& a);
    IntActivity& operator =(const IntActivity& a);
    IntActivity(Home home, const IntVarArgs& x, double d=1.0,
                IntBranchMerit bm=NULL);
    void init(Home home, const IntVarArgs& x, double d=1.0,
              IntBranchMerit bm=NULL);
  };
  inline
  IntActivity::IntActivity(void) {}
  inline
  IntActivity::IntActivity(const IntActivity& a)
    : Activity(a) {}
  inline IntActivity&
  IntActivity::operator =(const IntActivity& a) {
    return static_cast<IntActivity&>(Activity::operator =(a));
  }
  inline
  IntActivity::IntActivity(Home home, const IntVarArgs& x, double d,
                           IntBranchMerit bm) {
    ViewArray<Int::IntView> y(home,x);
    Activity::init(home,y,d,bm);
  }
  inline void
  IntActivity::init(Home home, const IntVarArgs& x, double d,
                    IntBracnhMerit bm) {
    ViewArray<Int::IntView> y(home,x);
    Activity::init(home,y,d,bm);
  }
}
*/


// Minimum branching classes
// Var branch
namespace MPG {
  /*
  class IntPairVarBranch : public VarBranch {
  public:
    enum Select {
      SEL_NONE
    };

  protected:
    Select s;
    
  public:
    IntPairVarBranch(void);
    IntPairVarBranch(Select s0, BranchTbl t);
    Select select(void) const;
  };

  inline
  IntPairVarBranch::IntPairVarBranch(void)
    : VarBranch(NULL), s(SEL_NONE) {}


  inline
  IntPairVarBranch::IntPairVarBranch(Select s0, BranchTbl t)
    : VarBranch(t), s(s0) {}

  inline
  IntPairVarBranch::Select IntPairVarBranch::select(void) const {
    return s;
  }

  IntPairVarBranch INTPAIR_VAR_NONE(void);

  inline IntPairVarBranch
  INTPAIR_VAR_NONE(void) {
    return IntPairVarBranch(IntPairVarBranch::SEL_NONE, NULL);
    } */

  // Val branch
  class IntPairValBranch : public ValBranch {
  public:
    enum Select {
      SEL_MIN, SEL_VAL_COMMIT
    };
  protected:
    Select s;
  public:
    IntPairValBranch(Select s0 = SEL_MIN)
      : s(s0) {}
    //    IntPairValBranch(VoidFunction v, VoidFunction c)
    //      : ValBranch(v,c), s(SEL_VAL_COMMIT) {}
    Select select(void) const {
      return s;
    }
  };
  IntPairValBranch INTPAIR_VAL_MIN(void);
  //  IntPairValBranch INTPAIR_VAL(IntPairBranchVal v, IntPairBranchCommit c=NULL);

  inline IntPairValBranch
  INTPAIR_VAL_MIN(void) {
    return IntPairValBranch(IntPairValBranch::SEL_MIN);
  }
  /*
    inline IntPairValBranch
    INTPAIR_VAL(IntPairBranchVal v, IntPairBranchCommit c) {
    return IntPairValBranch(function_cast<VoidFunction>(v),
    function_cast<VoidFunction>(c));
  }
  */
}





/*
namespace MPG {
  // variable selection class
  class IntVarBranch : public VarBranch {
  public:
    enum Select {
      SEL_NONE,       SEL_RND,          SEL_MERIT_MAX,
      SEL_DEGREE_MAX, SEL_ACTIVITY_MAX, SEL_SIZE_MIN
    };
  protected:
    Select s;
  public:
    IntVarBranch(void) ;
    IntVarBranch(Rnd r);
    IntVarBranch(Select s0, BranchTbl t);
    IntVarBranch(Select s0, double d, BranchTbl t);
    IntVarBranch(Select s0, Activity a, BranchTbl t);
    IntVarBranch(Select s0, VoidFunction mf, BranchTbl t);
    Select select(void) const;
    // expand activity
    void expand(Home home, const IntVarArgs& x) {
      if ((select() == SEL_ACTIVITY_MAX) &&
          !activity().initialized())
        activity(IntActivity(home,x,decay()));
    }
  };
  inline
  IntVarBranch::IntVarBranch(void) 
    : VarBranch(NULL), s(SEL_NONE) {}
  inline
  IntVarBranch::IntVarBranch(Rnd r) 
    : VarBranch(r), s(SEL_RND) {}
  inline
  IntVarBranch::IntVarBranch(Select s0, BranchTbl t)
    : VarBranch(t), s(s0) {}
  inline
  IntVarBranch::IntVarBranch(Select s0, double d, BranchTbl t)
    : VarBranch(d,t), s(s0) {}
  inline
  IntVarBranch::IntVarBranch(Select s0, Activity a, BranchTbl t)
    : VarBranch(a,t), s(s0) {}
  inline
  IntVarBranch::IntVarBranch(Select s0, VoidFunction mf, BranchTbl t)
    : VarBranch(mf,t), s(s0) {}
  inline
  IntVarBranch::Select IntVarBranch::select(void) const {
    return s;
  }
  // variable selection functions
  IntVarBranch INT_VAR_NONE(void);
  IntVarBranch INT_VAR_RND(Rnd r);
  IntVarBranch INT_VAR_MERIT_MAX(IntBranchMerit bm, 
                                 BranchTbl tbl=NULL);
  IntVarBranch INT_VAR_DEGREE_MAX(BranchTbl tbl=NULL);
  IntVarBranch INT_VAR_ACTIVITY_MAX(double d=1.0, 
                                    BranchTbl tbl=NULL);     
  IntVarBranch INT_VAR_ACTIVITY_MAX(IntActivity a, 
                                    BranchTbl tbl=NULL);     
  IntVarBranch INT_VAR_SIZE_MIN(BranchTbl tbl=NULL);
  // variable selection function implementation
  inline IntVarBranch
  INT_VAR_MERIT_MAX(IntBranchMerit bm, BranchTbl tbl) {
    return IntVarBranch(IntVarBranch::SEL_MERIT_MAX,
                        function_cast<VoidFunction>(bm),tbl);
  }
  inline IntVarBranch
  INT_VAR_NONE(void) {
    return IntVarBranch(IntVarBranch::SEL_NONE,NULL);
  }
  inline IntVarBranch
  INT_VAR_RND(Rnd r) {
    return IntVarBranch(r);
  }
  inline IntVarBranch
  INT_VAR_DEGREE_MAX(BranchTbl tbl) {
    return IntVarBranch(IntVarBranch::SEL_DEGREE_MAX,tbl);
  }
  inline IntVarBranch
  INT_VAR_ACTIVITY_MAX(double d, BranchTbl tbl) {
    return IntVarBranch(IntVarBranch::SEL_ACTIVITY_MAX,d,tbl);
  }
  inline IntVarBranch
  INT_VAR_ACTIVITY_MAX(IntActivity a, BranchTbl tbl) {
    return IntVarBranch(IntVarBranch::SEL_ACTIVITY_MAX,a,tbl);
  }
  inline IntVarBranch
  INT_VAR_SIZE_MIN(BranchTbl tbl) {
    return IntVarBranch(IntVarBranch::SEL_SIZE_MIN,tbl);
  }
  // value selection functions
  class IntValBranch : public ValBranch {
  public:
    enum Select {
      SEL_MIN, SEL_RND, SEL_VAL_COMMIT
    };
  protected:
    Select s;
  public:
    IntValBranch(Select s0 = SEL_MIN)
      : s(s0) {}
    IntValBranch(Rnd r)
      : ValBranch(r), s(SEL_RND) {}
    IntValBranch(VoidFunction v, VoidFunction c)
      : ValBranch(v,c), s(SEL_VAL_COMMIT) {}
    Select select(void) const {
      return s;
    }
  };
  IntValBranch INT_VAL_MIN(void);
  IntValBranch INT_VAL_RND(Rnd r);
  IntValBranch INT_VAL(IntBranchVal v, IntBranchCommit c=NULL);
  inline IntValBranch
  INT_VAL_MIN(void) {
    return IntValBranch(IntValBranch::SEL_MIN);
  }
  inline IntValBranch
  INT_VAL_RND(Rnd r) {
    return IntValBranch(r);
  }
  inline IntValBranch
  INT_VAL(IntBranchVal v, IntBranchCommit c) {
    return IntValBranch(function_cast<VoidFunction>(v),
                        function_cast<VoidFunction>(c));
  }
}

namespace MPG { namespace Int {
  // view selection creation function
  // size merit class
  class MeritSize : public MeritBase<IntView,unsigned int> {
  public:
    MeritSize(Space& home, const VarBranch& vb)
      : MeritBase<IntView,unsigned int>(home,vb) {}
    MeritSize(Space& home, bool share, MeritSize& m)
      : MeritBase<IntView,unsigned int>(home,share,m) {}
    unsigned int operator ()(const Space& home, IntView x, int i) {
      return x.max() - x.min();
    }
  };
  inline ViewSel<IntView>*
  viewsel(Space& home, const IntVarBranch& ivb) {
    if (ivb.select() == IntVarBranch::SEL_NONE)
      return new (home) ViewSelNone<IntView>(home,ivb);
    if (ivb.select() == IntVarBranch::SEL_RND)
      return new (home) ViewSelRnd<IntView>(home,ivb);
    if (ivb.tbl() != NULL) {
      // view selection with tbl-function
      switch (ivb.select()) {
      case IntVarBranch::SEL_MERIT_MAX:
        return new (home) ViewSelMaxTbl<MeritFunction<IntView> >(home,ivb);
      case IntVarBranch::SEL_DEGREE_MAX:
        return new (home) ViewSelMaxTbl<MeritDegree<IntView> >(home,ivb);
      case IntVarBranch::SEL_ACTIVITY_MAX:
        return new (home) ViewSelMaxTbl<MeritActivity<IntView> >(home,ivb);
      case IntVarBranch::SEL_SIZE_MIN:
        return new (home) ViewSelMinTbl<MeritSize>(home,ivb);
      default: ;
      }
    } else {
      // view selection without tbl-function
      switch (ivb.select()) {
      case IntVarBranch::SEL_MERIT_MAX:
        return new (home) ViewSelMax<MeritFunction<IntView> >(home,ivb);
      case IntVarBranch::SEL_DEGREE_MAX:
        return new (home) ViewSelMax<MeritDegree<IntView> >(home,ivb);
      case IntVarBranch::SEL_ACTIVITY_MAX:
        return new (home) ViewSelMax<MeritActivity<IntView> >(home,ivb);
      case IntVarBranch::SEL_SIZE_MIN:
        return new (home) ViewSelMin<MeritSize>(home,ivb);
      default: ;
      }
    }
    throw UnknownBranching("Int::branch");
  }
  // value selection and commit creation function
  // value selection classes
  class ValSelMin : public ValSel<IntView,int> {
  public:
    ValSelMin(Space& home, const ValBranch& vb)
      : ValSel<IntView,int>(home,vb) {}
    ValSelMin(Space& home, bool share, ValSelMin& vs)
      : ValSel<IntView,int>(home,share,vs) {}
    int val(const Space& home, IntView x, int i) {
      return x.min();
    }
  };
  class ValSelRnd : public ValSel<IntView,int> {
  protected:
    Rnd r;
  public:
    ValSelRnd(Space& home, const ValBranch& vb)
      : ValSel<IntView,int>(home,vb), r(vb.rnd()) {}
    ValSelRnd(Space& home, bool share, ValSelRnd& vs)
      : ValSel<IntView,int>(home,share,vs) {
      r.update(home,share,vs.r);
    }
    int val(const Space& home, IntView x, int i) {
      return x.min() + r(x.max() - x.min());
    }
    bool notice(void) const {
      return true;
    }
    void dispose(Space& home) {
      r.~Rnd();
    }
  };
  // value commit class
  // no-good literal class
  class LqNGL : public ViewValNGL<IntView,int,PC_INT_BND> {
    using ViewValNGL<IntView,int,PC_INT_BND>::x;
    using ViewValNGL<IntView,int,PC_INT_BND>::n;
  public:
    LqNGL(Space& home, IntView x, int n);
    LqNGL(Space& home, bool share, LqNGL& ngl);
    virtual NGL* copy(Space& home, bool share);
    virtual NGL::Status status(const Space& home) const;
    virtual ExecStatus prune(Space& home);
  };
  inline
  LqNGL::LqNGL(Space& home, IntView x, int n)
    : ViewValNGL<IntView,int,PC_INT_BND>(home,x,n) {}
  inline
  LqNGL::LqNGL(Space& home, bool share, LqNGL& ngl)
    : ViewValNGL<IntView,int,PC_INT_BND>(home,share,ngl) {}
  inline NGL*
  LqNGL::copy(Space& home, bool share) {
    return new (home) LqNGL(home,share,*this);
  }
  inline NGL::Status
  LqNGL::status(const Space&) const {
    if (x.max() <= n)
      return NGL::SUBSUMED;
    else if (x.min() > n)
      return NGL::FAILED;
    else
      return NGL::NONE;
  }
  inline ExecStatus
  LqNGL::prune(Space& home) {
    return me_failed(x.gq(home,n+1)) ? ES_FAILED : ES_OK;
  }
  class ValCommitLq : public ValCommit<IntView,int> {
  public:
    ValCommitLq(Space& home, const ValBranch& vb)
      : ValCommit<IntView,int>(home,vb) {}
    ValCommitLq(Space& home, bool share, ValCommitLq& vc)
      : ValCommit<IntView,int>(home,share,vc) {}
    ModEvent commit(Space& home, unsigned int a, 
                    IntView x, int i, int n) {
      return (a == 0) ? x.lq(home,n) : x.gq(home,n+1);
    }
    void print(const Space&, unsigned int a, 
               IntView, int i, int n, 
               std::ostream& o) const {
	       o << "x[" << i << "] " 
	       << ((a == 0) ? "<=" : ">") << " " << n;
    }
    // no-good literal creation
    NGL* ngl(Space& home, unsigned int a, 
             IntView x, int n) const {
      if (a == 0)
        return new (home) LqNGL(home,x,n);
      else
        return NULL;
    }
  };
  inline ValSelCommitBase<IntView,int>* 
  valselcommit(Space& home, const IntValBranch& ivb) {
    switch (ivb.select()) {
    case IntValBranch::SEL_MIN:
      return new (home) 
        ValSelCommit<ValSelMin,ValCommitLq>(home,ivb);
    case IntValBranch::SEL_RND:
      return new (home) 
        ValSelCommit<ValSelRnd,ValCommitLq>(home,ivb);
    case IntValBranch::SEL_VAL_COMMIT:
     // user-defined value selection and commit functions
      if (ivb.commit() == NULL) {
        return new (home) 
          ValSelCommit<ValSelFunction<IntView>,
                       ValCommitLq>(home,ivb);
      } else {
        return new (home) 
          ValSelCommit<ValSelFunction<IntView>,
                       ValCommitFunction<IntView> >(home,ivb);
      }
    default:
      throw UnknownBranching("Int::branch");
    }
  }
}}

namespace MPG {
  // branch function
  inline BrancherHandle
  branch(Home home, const IntVarArgs& x,
         IntVarBranch vars, IntValBranch vals, 
         IntBranchFilter bf=NULL,
         IntVarValPrint vvp=NULL) {
    using namespace Int;
    if (home.failed()) return BrancherHandle();
    vars.expand(home,x);
    ViewArray<IntView> xv(home,x);
    ViewSel<IntView>* vs[1] = { 
      viewsel(home,vars) 
    };
    return ViewValBrancher<IntView,1,int,2>::post
      (home,xv,vs,valselcommit(home,vals),bf,vvp);
  }
  // branch function with tie-breaking
  inline BrancherHandle
  branch(Home home, const IntVarArgs& x,
         TieBreak<IntVarBranch> vars, IntValBranch vals,
         IntBranchFilter bf=NULL,
         IntVarValPrint vvp=NULL) {
    using namespace Int;
    if (home.failed()) return BrancherHandle();
    vars.a.expand(home,x);
    // normalizing tie-breaking
    if ((vars.a.select() == IntVarBranch::SEL_NONE) ||
        (vars.a.select() == IntVarBranch::SEL_RND))
      vars.b = INT_VAR_NONE();
    vars.b.expand(home,x);
    if ((vars.b.select() == IntVarBranch::SEL_NONE) ||
        (vars.b.select() == IntVarBranch::SEL_RND))
      vars.c = INT_VAR_NONE();
    vars.c.expand(home,x);
    if ((vars.c.select() == IntVarBranch::SEL_NONE) ||
        (vars.c.select() == IntVarBranch::SEL_RND))
      vars.d = INT_VAR_NONE();
    vars.d.expand(home,x);
    ViewArray<IntView> xv(home,x);
    if (vars.b.select() == IntVarBranch::SEL_NONE) {
      ViewSel<IntView>* vs[1] = { 
        viewsel(home,vars.a)
      };
      return ViewValBrancher<IntView,1,int,2>
        ::post(home,xv,vs,valselcommit(home,vals),bf,vvp);
    } else if (vars.c.select() == IntVarBranch::SEL_NONE) {
      ViewSel<IntView>* vs[2] = { 
        viewsel(home,vars.a), viewsel(home,vars.b)
      };
      return ViewValBrancher<IntView,2,int,2>
        ::post(home,xv,vs,valselcommit(home,vals),bf,vvp);
    } else if (vars.d.select() == IntVarBranch::SEL_NONE) {
      ViewSel<IntView>* vs[3] = { 
        viewsel(home,vars.a), viewsel(home,vars.b),
        viewsel(home,vars.c)
      };
      return ViewValBrancher<IntView,3,int,2>
        ::post(home,xv,vs,valselcommit(home,vals),bf,vvp);
    } else {
      ViewSel<IntView>* vs[4] = { 
        viewsel(home,vars.a), viewsel(home,vars.b),
        viewsel(home,vars.c), viewsel(home,vars.d)
      };         
      return ViewValBrancher<IntView,4,int,2>
        ::post(home,xv,vs,valselcommit(home,vals),bf,vvp);
    }
  }
}
*/
