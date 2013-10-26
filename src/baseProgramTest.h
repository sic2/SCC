#include <stdio.h>

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

#include "OPERATOR.h"
#include "ALT.h"
#include "EXPR.h"
#include "PROGRAM.h"
#include "TYPE.h"

using namespace AST; // XXX - possible omit 

// Define a constructor used by all the testing classes on loading
class baseProgramTest
{
public:
	inline baseProgramTest()
	{
		value_TRUE = boost::variant< bool >(true);
		value_FALSE = boost::variant< bool >(false);

		value_ZERO = boost::variant< int >(0);
		value_ONE = boost::variant< int >(1);
		value_TWO = boost::variant< int >(2);
		value_THREE = boost::variant< int >(3);

		op_add = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_ADDITION));
		op_sub = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_SUBTRACTION));
		op_mul = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_MULTIPLICATION));
		op_div = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_DIVISION));
		op_eq = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_EQUALITY));
		op_less_than = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_LESS));
		op_or = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_OR));
		op_and = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_AND));

		expr_TRUE = boost::shared_ptr<EXPR> (new EXPR(EXPR_BOOL, value_TRUE));
		expr_FALSE = boost::shared_ptr<EXPR> (new EXPR(EXPR_BOOL, value_FALSE));
	}

	virtual ~baseProgramTest() {}

protected:
	boost::variant< bool > value_TRUE;
	boost::variant< bool > value_FALSE;

	boost::variant< int > value_ZERO;
	boost::variant< int > value_ONE;
	boost::variant< int > value_TWO;
	boost::variant< int > value_THREE;

	boost::shared_ptr<OPERATOR> op_add;
	boost::shared_ptr<OPERATOR> op_sub;
	boost::shared_ptr<OPERATOR> op_mul;
	boost::shared_ptr<OPERATOR> op_div;
	boost::shared_ptr<OPERATOR> op_eq;
	boost::shared_ptr<OPERATOR> op_less_than;
	boost::shared_ptr<OPERATOR> op_or;
	boost::shared_ptr<OPERATOR> op_and;

	boost::shared_ptr<EXPR> expr_TRUE;
	boost::shared_ptr<EXPR> expr_FALSE;
};

// boost::shared_ptr<EXPR> expr_0(new EXPR(EXPR_INT, value_0));
// boost::shared_ptr<EXPR> expr_1(new EXPR(EXPR_INT, value_1));
// boost::shared_ptr<EXPR> expr_2(new EXPR(EXPR_INT, value_2));


// boost::shared_ptr<TYPE> type_0(new TYPE(TYPE_INT));
// boost::shared_ptr<TYPE> type_1(new TYPE(TYPE_INT));

// // Create alternatives one by one and add them to vector of alternatives
// std::vector< boost::shared_ptr<ALT> > alternatives;
// boost::shared_ptr<ALT> alt_0(new ALT(&expr_0, &expr_1)); 
// boost::shared_ptr<ALT> alt_1(new ALT(&expr_1, &expr_0)); 