#include <gtest.h>
#include "doublelist.h"

TEST(T_DNode, can_be_created)
{
	ASSERT_NO_THROW(T_DNode<int> A);
}