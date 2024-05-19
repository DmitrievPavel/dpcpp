#include <iostream>
#include <vector>
#include <map>
struct Base {
	std::string from;
	std::vector<std::string> column;
	std::map<std::string, std::string> where;
};

class SqlSelectQueryBuilder {
private:
	Base base;
public:
	SqlSelectQueryBuilder() {};

	SqlSelectQueryBuilder& AddFrom(const std::string& tablename) {
		base.from = tablename;
		return *this;
	}

	SqlSelectQueryBuilder& AddColumn(const std::string& column) {
		base.column.push_back(column);
		return *this;
	}

	SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value) {
		base.where[key] = value;
		return *this;
	}

	SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
		base.where = kv;
	}

	SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept {
		base.column = columns;
	}

	std::string BuildQuery() {
		std::string query = "SELECT ";

		if (base.column.empty()) {
			query += "*";
		}
		else {
			for (size_t i = 0; i < base.column.size(); ++i) {
				query += base.column[i];
				if (base.column.size() > 1 && i < base.column.size() - 1) {
					query += ", ";
				}
			}
		}

		query += " FROM " + base.from + " ";
		query += "WHERE ";
		int i = 0;

		for (const auto& w : base.where) {
			query += w.first + "=" + w.second;
			++i;
			if (base.where.size() > 1 && i < base.where.size()) {
				query += " AND ";
			}
		}
		query += ";";
		return query;
	}
};

int main()
{
	SqlSelectQueryBuilder query_builder;
	query_builder.AddColumn("name").AddColumn("phone");
	query_builder.AddFrom("students");
	query_builder.AddWhere("id", "42").AddWhere("name", "John");
	std::cout << query_builder.BuildQuery();
	return 0;
}
