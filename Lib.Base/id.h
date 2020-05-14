#pragma once
#include <cstdint>
#include "CommonHelper.h"
#include <algorithm>
#include <memory.h>

struct ID final
{
	static constexpr int32_t ID_LENGTH = 50;
	uint32_t				 idSize = 0;
	char					 id[ID_LENGTH] = { 0 };

	ID() = default;
	~ID() = default;

	ID(const ID& _id)
	{
		assign(_id.id, _id.idSize);
	}

	ID(ID&& _id) noexcept
	{
		std::swap(id, _id.id);
		std::swap(idSize, _id.idSize);
	}

	ID(const char *pId, uint32_t size)
	{
		assign(pId, size);
	}

	bool assign(const char *pId, uint32_t size)
	{
		clear();
		if (pId != nullptr && size < ID_LENGTH)
		{
			memcpy_s(id, sizeof(id), pId, size);
			idSize = size;
			return true;
		}
		idSize = 0;
		return false;
	}

	bool assign(char c, uint32_t number)
	{
		if (number <= ID_LENGTH)
		{
			memset(id, c, number);
			idSize = number;
			return true;
		}
		idSize = 0;
		return false;
	}

	int compare(const ID & value) const
	{
		return memcmp(id, value.id, std::max<uint32_t>(idSize, value.idSize));
	}

	bool empty() const
	{
		return idSize == 0;
	}

	ID& operator=(const ID & _id)
	{
		if (this != &_id)
		{
			assign(_id.id, _id.idSize);
		}
		return *this;
	}

	ID& operator=(ID && _id) noexcept
	{
		if (this != &_id)
		{
			std::swap(id, _id.id);
			std::swap(idSize, _id.idSize);
		}
		return *this;
	}

	bool operator==(const ID & value) const
	{
		return compare(value) == 0;
	}

	bool operator!=(const ID & value) const
	{
		return compare(value) != 0;
	}

	bool operator < (const ID & cmp) const
	{
		return compare(cmp) < 0;
	}

	static ID make_ID(const char *pId, uint32_t size)
	{
		ID id(pId, size);
		return id;
	}

	void clear()
	{
		memset(id, 0, sizeof(id));
		idSize = 0;
	}

	std::string string() const
	{
		return ::getIDString(id, idSize);
	}

	std::wstring wstring() const
	{
		return ::getIDWString(id, idSize);
	}
};
