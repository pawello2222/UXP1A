//
// Created by Pawel Wiszenko on 28.04.2016.
//

#include <iostream>
#include "Model/LindaTupleItem.h"
#include "Model/LindaTuple.h"
#include "Model/LindaTupleTemplate.h"
#include "IPC/LindaTuplePool.h"
#include "Exception/LindaTuplePoolConnectionError.h"

using namespace std;

int main()
{
    LindaTuple lt1 = LindaTuple({LindaTupleItem(123)});
    LindaTuple lt2 = LindaTuple({LindaTupleItem(124)});
    LindaTuple lt3 = LindaTuple({LindaTupleItem("123")});
    LindaTupleTemplate tt = LindaTupleTemplate({LindaTupleItemTemplate(LindaTupleItemType::Integer, "123")});
    std::cout << "Expected 1, got: " << tt.IsMatch(lt1) << std::endl;
    std::cout << "Expected 0, got: " <<tt.IsMatch(lt2) << std::endl;
    std::cout << "Expected 0, got: " <<tt.IsMatch(lt3) << std::endl;

    LindaTuplePool pool;

    try
    {
        pool.ConnectPool("/tmp/tuples", "/tmp/queue");
    }
    catch (LindaTuplePoolConnectionError ex)
    {
        std::cout << ex.GetMessage() << " " << ex.GetErrorCode() << std::endl;
    }

    return 0;
}
