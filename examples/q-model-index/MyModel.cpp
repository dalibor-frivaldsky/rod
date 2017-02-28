// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include "MyModel.hpp"


namespace example {

	MyModel::MyModel():
		modelData({
			RowData{
				10,
				StringData{ "abc" }
			},
			RowData{
				20,
				StringData{ "def" }
			}
		})
	{}

	QModelIndex
	MyModel::index(int row, int col, const QModelIndex&) const {
		return this->createIndex(row, col);
	}

	QModelIndex
	MyModel::parent(const QModelIndex&) const {
		return QModelIndex{};
	}

	int
	MyModel::rowCount(const QModelIndex&) const {
		return this->modelData.size();
	}

	int
	MyModel::columnCount(const QModelIndex&) const {
		return 1;
	}

	QVariant
	MyModel::data(const QModelIndex& index, int role) const {
		int	r = index.row();

		if(role == IntRole) {
			return QVariant::fromValue(this->modelData[r].i);
		}
		else if(role == StringDataRole) {
			return QVariant::fromValue(this->modelData[r].stringData);
		}
		else {
			return QVariant{};
		}
	}

	MyIndex
	MyModel::myIndex(int row, int col) const {
		return MyIndex{ this->index(row, col) };
	}

}