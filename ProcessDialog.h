#pragma once

#include <QDialog>
#include "ProcessData.h"
class QSpinBox;
class QLineEdit;
class QTextEdit;
class QDialogButtonBox;

class ProcessDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY( ProcessDialog )
public:
    explicit ProcessDialog( QWidget* parent = nullptr );
    void setData( const ProcessData& );
    ProcessData data() const;

private:
    Q_SLOT void onEdit();

private:
    QSpinBox*const m_row = nullptr;
    QSpinBox*const m_column = nullptr;
    QLineEdit*const m_title = nullptr;
    QTextEdit*const m_description = nullptr;
    QLineEdit*const m_command = nullptr;
    QDialogButtonBox* const m_buttons = nullptr;
    ProcessData m_original_data;
};
