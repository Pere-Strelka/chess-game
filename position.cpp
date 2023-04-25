#include "position.h"
#include "piece.h"

/*

The encode() function encodes given matrix using codes from table in the position.h file.
It starts from 1st square of matrix which is considered to be a8 and encodes every square 
by the code of its piece and side if one is found and 0 if the square is empty. Function 
firstly fills the x1 variable and then proceeds to the x2, x3 and x4 if needed.

*/

PositionCode::PositionCode(unsigned long long x1, unsigned long long x2, unsigned long long x3, unsigned long long x4)
    : _x1{ x1 }
    , _x2{ x2 }
    , _x3{ x3 }
    , _x4{ x4 }
{}

PositionCode PositionCode::encode(ChessMatrix& matrix)
{
    auto get_codeLength = [](short code) 
    {
        if (code == 0b0) return 1;
        if (code == 0b100 || code == 0b101) return 3;
        return 4;
    };

    QVector<long long> codes_8bytes;
    codes_8bytes.reserve(4);
    short length_8bytes = 0;
    long long code_8bytes = 0;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            short code, code_length;
            code = _code_table.at(matrix(i, j).pieceType());
            code_length = get_codeLength(code);
            if (code)
            {
                if (matrix(i, j).piece()->side() == Side::White)
                {
                    short side_bit = 1 << code_length;
                    code += side_bit;
                }
                code_length++;    
            }
            if (length_8bytes + code_length > 64)
            {
                // If 8 bytes are about to overflow, we need to cut the current code 
                // and write one part in the current 8 bytes and then write the 
                // remaining part into new 8 bytes.

                short toMove = 64 - length_8bytes;
                code_8bytes = code_8bytes << toMove;
                toMove = code_length - toMove;
                short code_copy = code >> toMove;
                code_8bytes += code_copy;
                codes_8bytes.append(code_8bytes);
                length_8bytes += toMove;
                code_8bytes = code;
            }
            else
            {
                code_8bytes = code_8bytes << code_length;
                code_8bytes += code;
                length_8bytes += code_length;
            }
        }
    }
    return PositionCode(codes_8bytes[0], codes_8bytes[1], codes_8bytes[2], codes_8bytes[3]);
}
