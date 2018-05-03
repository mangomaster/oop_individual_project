#ifndef SOLVER_H_
#define SOLVER_H_

#include <string>
#include <cassert>
#include "profile.h"
#include "z3++.h"

namespace DMFB
{
/**
 * @brief DMFB solver
 * 
 */
class Solver
{
public:
  Solver(const Profile &p) : pf(p), objective(""){};
  ~Solver(){};

  void solve();
  void print();

  const std::string &getObjective() const
  {
    return this->objective;
  }
  void setObjective(const std::string &o)
  {
    assert(o.compare("prove") == 0 || o.compare("min time") == 0 || o.compare("min size") == 0);
    objective = o;
  }

private:
  Profile pf;
  std::string objective;

  void _solve();

  class Formulator;
  class Prover;

  const int MAXTIME = 20;
  const int MAXLENGTH = 8;
};

/**
 * @brief Formulate configuration and profile into z3Solver context.
 * 
 */
class Solver::Formulator
{
public:
  Formulator(const Profile &p);
  ~Formulator(){};

  inline z3::context &getContext() const;
  inline const std::vector<z3::expr_vector> &getExprVector() const;
  inline const int getNetNum() const;
  inline const void getSizeNum(int &x, int &y) const;
  inline const int getTNum() const;
  const void computeDroplet(int &dimension1, std::vector<int> &dimension2, int net, int x, int y, int time) const;
  const void computeDetector(int &dimension1, int &dimension2, int net, int x, int y) const;
  const void computeDispenser(int &dimension1, int &dimension2, int xEdge, int yEdge) const;
  const void computeSinker(int &dimension1, int &dimension2, int xEdge, int yEdge) const;
  void formulate();

private:
  const Profile &pf;
  z3::context cxt;
  std::vector<z3::expr_vector> exprVec;

  int netNum;
  int xNum, yNum;
  int tNum;
  int edgeNum;

  void formulateDroplet();
  void formulateDetector();
  void formulateDispenser();
  void formulateSinker();
  void formulateDetecting();

  Formulator(Formulator &);
  Formulator &operator=(Formulator &);
};

/**
 * @brief prove the satisfiability using z3Solver solver.
 * 
 */
class Solver::Prover
{
public:
  Prover(Formulator &f);
  ~Prover(){};

  void prove();

private:
  Formulator &formu;
  z3::solver solv;

  z3::context &cxt;
  const std::vector<z3::expr_vector> &exprVec;
  int netNum;
  int xNum, yNum;
  int tNum;

  void noOverlap();
  void noSpaceNeighbor();
  void noTimeNeighbor();
  void enoughSize();
  void operationMovement();
  void operationMixing();
  void operationDetecting();
  void meetObjective();

  Prover(Prover &);
  Prover &operator=(Prover &);
};

/*******************functions********************/
inline z3::context &Solver::Formulator::getContext() const
{
  return const_cast<z3::context &>(this->cxt);
}

inline const std::vector<z3::expr_vector> &Solver::Formulator::getExprVector() const
{
  return this->exprVec;
}

inline const int Solver::Formulator::getNetNum() const
{
  return this->netNum;
}

inline const void Solver::Formulator::getSizeNum(int &x, int &y) const
{
  x = xNum;
  y = yNum;
}

inline const int Solver::Formulator::getTNum() const
{
  return this->tNum;
}

}; // namespace DMFB

#endif // SOLVER_H_