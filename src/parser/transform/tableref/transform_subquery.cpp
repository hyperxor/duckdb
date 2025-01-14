#include "duckdb/parser/tableref/subqueryref.hpp"
#include "duckdb/parser/transformer.hpp"

using namespace duckdb;
using namespace std;

unique_ptr<TableRef> Transformer::TransformRangeSubselect(postgres::RangeSubselect *root) {
	auto subquery = TransformSelectNode((postgres::SelectStmt *)root->subquery);
	if (!subquery) {
		return nullptr;
	}
	auto result = make_unique<SubqueryRef>(move(subquery));
	result->alias = TransformAlias(root->alias);
	if (root->alias->colnames) {
		for (auto node = root->alias->colnames->head; node != nullptr; node = node->next) {
			result->column_name_alias.push_back(reinterpret_cast<postgres::Value *>(node->data.ptr_value)->val.str);
		}
	}
	return move(result);
}
