#include "duckdb/parser/statement/alter_table_statement.hpp"
#include "duckdb/parser/transformer.hpp"

using namespace duckdb;
using namespace std;

unique_ptr<AlterTableStatement> Transformer::TransformRename(postgres::Node *node) {
	auto stmt = reinterpret_cast<postgres::RenameStmt *>(node);
	assert(stmt);
	assert(stmt->relation);

	unique_ptr<AlterTableInfo> info;

	// first we check the type of ALTER
	switch (stmt->renameType) {
	case postgres::OBJECT_COLUMN: {
		// change column name

		// get the table and schema
		string schema = DEFAULT_SCHEMA;
		string table;
		assert(stmt->relation->relname);
		if (stmt->relation->relname) {
			table = stmt->relation->relname;
		}
		if (stmt->relation->schemaname) {
			schema = stmt->relation->schemaname;
		}
		// get the old name and the new name
		string old_name = stmt->subname;
		string new_name = stmt->newname;
		info = make_unique<RenameColumnInfo>(schema, table, old_name, new_name);
		break;
	}
	case postgres::OBJECT_DATABASE:
	default:
		throw NotImplementedException("Schema element not supported yet!");
	}
	assert(info);
	return make_unique<AlterTableStatement>(move(info));
}
