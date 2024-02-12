#ifndef MINISQL_EXECUTE_ENGINE_H
#define MINISQL_EXECUTE_ENGINE_H

#include <memory>
#include <string>
#include <unordered_map>
#include "parser/syntax_tree_printer.h"
#include "common/dberr.h"
#include "common/instance.h"
#include "executor/execute_context.h"
#include "executor/executors/abstract_executor.h"
#include "executor/plans/abstract_plan.h"
#include "record/row.h"
#include "transaction/transaction.h"

extern "C"
{
extern int yyparse(void);
extern FILE *yyin;
#include "../parser/minisql_lex.h"
#include "../parser/parser.h"
};

/**
 * ExecuteEngine
 */
class ExecuteEngine {
 public:
  ExecuteEngine();

  ~ExecuteEngine() {
    for (auto it : dbs_) {
      delete it.second;
    }
  }

  /**
   * executor interface
   */
  dberr_t Execute(pSyntaxNode ast);

  dberr_t ExecutePlan(const AbstractPlanNodeRef &plan, std::vector<Row> *result_set, Transaction *txn,
                      ExecuteContext *exec_ctx);

  void ExecuteInformation(dberr_t result);

  void  AstToVector(pSyntaxNode ast,vector<string>&vector1){

    if (ast->val_ != nullptr) {
      vector1.push_back(ast->val_);
    }

    if (ast->child_ != nullptr) {
      AstToVector(ast->child_, vector1);
    }

    if (ast->next_ != nullptr) {
      AstToVector(ast->next_, vector1);

    }
  }

 private:
  static std::unique_ptr<AbstractExecutor> CreateExecutor(ExecuteContext *exec_ctx, const AbstractPlanNodeRef &plan);

  dberr_t ExecuteCreateDatabase(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteDropDatabase(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteShowDatabases(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteUseDatabase(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteShowTables(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteCreateTable(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteDropTable(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteShowIndexes(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteCreateIndex(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteDropIndex(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteTrxBegin(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteTrxCommit(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteTrxRollback(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteExecfile(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteQuit(pSyntaxNode ast, ExecuteContext *context);

 private:
  std::unordered_map<std::string, DBStorageEngine *> dbs_; /** all opened databases */
  std::string current_db_;                                 /** current database */
};

#endif  // MINISQL_EXECUTE_ENGINE_H
