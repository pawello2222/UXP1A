//
// Created by Pawel Wiszenko on 28.04.2016.
//

#include <iostream>
#include "Model/LindaTupleItem.h"
#include "Model/LindaTuple.h"
#include "Model/LindaTupleTemplate.h"
#include "IPC/LindaTuplePool.h"
#include "Exception/LindaTuplePoolConnectionError.h"
#include "ExpressionParser/LindaTupleParser.h"
#include "ExpressionParser/LindaTemplateParser.h"
#include "Exception/UnknownLindaTupleOperator.h"
#include "Exception/UnknownLindaTupleType.h"

using namespace std;

int main()
{
    LindaTuple lt1 = LindaTuple({LindaTupleItem(123)});
    LindaTuple lt2 = LindaTuple({LindaTupleItem(123), LindaTupleItem("123")});
    LindaTuple lt3 = LindaTuple({LindaTupleItem((float)123), LindaTupleItem("124")});
    LindaTupleTemplate tt = LindaTupleTemplate({LindaTupleItemTemplate(LindaTupleItemType::Float, LindaTupleItemOperator::ge, "123"),
                                                LindaTupleItemTemplate(LindaTupleItemType::String, LindaTupleItemOperator::gt, "123")});


//    char arr[255];
//    LindaTuplesFileEntry entry;
//    strcpy(entry.TupleData, "(1.13, \"123\", 3)");
//    LindaTupleParser parser(entry);
//    auto tuple = parser.parse();

    char arr2[255];
    LindaTuplesFileEntry entry2;
    strcpy(entry2.TupleData, "(integer:== 3,string: *,float: >=3.14)");
    LindaTemplateParser templateParser(entry2);
    auto templateParsed = templateParser.parse();

    //tt.IsMatch(lt1);
    //tt.IsMatch(lt2);
    //tt.IsMatch(lt1);
    try
    {
        std::cout << "Expected 0, got: " << tt.IsMatch(lt1) << std::endl;
        std::cout << "Expected 0, got: " << tt.IsMatch(lt2) << std::endl;
        std::cout << "Expected 1, got: " << tt.IsMatch(lt3) << std::endl;
    }
    catch ( UnknownLindaTupleType& e )
    {
        std::cout << "Error: Invalid tuple type" << std::endl;
    }
    catch ( UnknownLindaTupleOperator& e )
    {
        std::cout << "Error: Invalid tuple operator" << std::endl;
    }


    LindaTuplePool pool;

    try
    {
        pool.ConnectPool("/tmp/tuples", "/tmp/queue");
    }
    catch (LindaTuplePoolConnectionError ex)
    {
        std::cout << ex.GetMessage() << " " << ex.GetErrorCode() << std::endl;
        return 1;
    }

    pool.Output(lt3);
    pool.Output(lt1);

    return 0;
}
