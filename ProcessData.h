#pragma once

#include <QString>

struct ProcessData {
    int row = 0;
    int column = 0;
    QString title;
    QString description;
    QString command;

    bool operator==( const ProcessData& other ) const {
        if ( &other == this ) {
            return true;
        }

        bool ok = ( other.row == row );
        ok = ok && ( other.column == column );
        ok = ok && ( other.title == title );
        ok = ok && ( other.title == title );
        ok = ok && ( other.description == description );
        ok = ok && ( other.command == command );

        return ok;
    }

    bool operator!=( const ProcessData& other ) const {
        return not operator==( other );
    }
};
