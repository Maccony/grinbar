#include "ProcessDialog.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QTextEdit>
#include <QPushButton>

ProcessDialog::ProcessDialog( QWidget* parent )
    : QDialog( parent )
    , m_row( new QSpinBox( this ) )
    , m_column( new QSpinBox( this ) )
    , m_title( new QLineEdit( this ) )
    , m_description( new QTextEdit( this ) )
    , m_command( new QLineEdit( this ) )
    , m_buttons( new QDialogButtonBox( this ) )
{
    setWindowTitle( tr( "Add Сommand" ) );
    auto layout = new QVBoxLayout( this );

    auto position_group = new QGroupBox( tr( "Position" ), this );
    auto h_layout = new QHBoxLayout( position_group );

    auto makeLabel = []( const QString& text ) -> QLabel* {
        auto label = new QLabel( text );
        label->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred );
        return label;
    };
    h_layout->addWidget( makeLabel( tr( "Row" ) ) );
    connect( m_row, SIGNAL(valueChanged(int)), this, SLOT(onEdit()) );
    h_layout->addWidget( m_row );
    h_layout->addSpacing( 40 );
    h_layout->addWidget( makeLabel( tr( "Column" ) ) );
    h_layout->addWidget( m_column );
    connect( m_column, SIGNAL(valueChanged(int)), this, SLOT(onEdit()) );
    layout->addWidget( position_group );

    auto title_group = new QGroupBox( tr( "Title" ), this );
    layout->addWidget( title_group );
    title_group->setLayout( new QVBoxLayout );
    title_group->layout()->addWidget( m_title );
    connect( m_title, &QLineEdit::textChanged, this, &ProcessDialog::onEdit );
    layout->addWidget( title_group );

    auto description_group = new QGroupBox( tr( "Description" ), this );
    description_group->setLayout( new QVBoxLayout );
    description_group->layout()->addWidget( m_description );
    connect( m_description, &QTextEdit::textChanged, this, &ProcessDialog::onEdit );
    layout->addWidget( description_group );

    auto command_group = new QGroupBox( tr( "Command" ), this );
    command_group->setLayout( new QVBoxLayout );
    command_group->layout()->addWidget( m_command );
    layout->addWidget( command_group );

    m_buttons->addButton( QDialogButtonBox::Ok );
    m_buttons->button( QDialogButtonBox::Ok )->setEnabled( false );
    m_buttons->addButton( QDialogButtonBox::Cancel );
    m_buttons->button( QDialogButtonBox::Cancel )->setDefault( true );
    connect( m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept );
    connect( m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject );
    layout->addWidget( m_buttons );
}

void ProcessDialog::setData( const ProcessData& original ) {
    m_original_data = original;
    m_row->setValue( original.row );
    m_column->setValue( original.column );
    m_title->setText( original.title );
    m_description->setHtml( original.description );
    m_command->setText( original.command );
    onEdit();
}

ProcessData ProcessDialog::data() const {
    auto res = m_original_data;
    res.row = m_row->value();
    res.column = m_column->value();
    res.title = m_title->text();
    res.description = m_description->toHtml();
    res.command = m_command->text();
    return res;
}

void ProcessDialog::onEdit() {
    const auto actual = data();
    const bool changed = actual != m_original_data;
    m_buttons->button( QDialogButtonBox::Ok )->setEnabled( changed );
}
