#include "task.h"
#include "logged_command_wrapper.h"
#include <cassert>
#include <iostream>

/* Курсор влево */
class MoveCursorLeftCommand : public ICommand {
public:
    MoveCursorLeftCommand () {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        if (cursorPosition == 0)
            return;
        --cursorPosition;
        if (buffer[cursorPosition] == '\n') {
            --cursorPosition;
        }
    }

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitMoveCursorLeftCommand(*this);
    }

private:
    std::ostream* logStreamPtr_;

};

/* Курсор вправо */
class MoveCursorRightCommand : public ICommand {
public:
    MoveCursorRightCommand() {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        if (cursorPosition == buffer.size())
            return;
        ++cursorPosition;
        if (buffer[cursorPosition] == '\n') {
            ++cursorPosition;
        }
    }

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitMoveCursorRightCommand(*this);
    }
};

/* Курсор вверх */
class MoveCursorUpCommand : public ICommand {
public:
    MoveCursorUpCommand () {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        if (cursorPosition == 0)
            return;
        
        bool f = true;
        size_t copy = cursorPosition;
        while (copy > 0) {
            --copy;
            if (buffer[copy] == '\n')
                break;
        }
        int razn = cursorPosition-copy;
        if (buffer[copy] == '\n')
            f = false;
        while (copy > 0) {
            --copy;
            if (buffer[copy] == '\n')
                break;
        }
    
        cursorPosition = copy+razn;
        if (!f && copy == 0)
            --cursorPosition;

            
    }

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitMoveCursorUpCommand(*this);
    }
};

/* Курсор вниз */
class MoveCursorDownCommand : public ICommand {
public:
    MoveCursorDownCommand() {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        if (cursorPosition == buffer.size())
            return;

        bool f = true;
        size_t copy = cursorPosition;
        while (copy > 0) {
            --copy;
            if (buffer[copy] == '\n')
                break;
        }
        if (buffer[copy] == '\n')
            f = false;

        int razn = cursorPosition-copy;
        while (copy < buffer.size()) {
            ++copy;
            if (buffer[copy] == '\n')
                break;
        }

        cursorPosition = copy+razn;
        if (f)
            cursorPosition++;
    }

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitMoveCursorDownCommand(*this);
    }
};

/* Выделить текст */
class SelectTextCommand : public ICommand {
public:
    SelectTextCommand(size_t& selectionSize) : selectionSize_(selectionSize) {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        editor.SelectText(cursorPosition, cursorPosition+selectionSize_);
    }

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitSelectCommand(*this);
    }
private:
    size_t selectionSize_;
};

/* Ввести текст */
class InsertTextCommand : public ICommand {
public: 
    InsertTextCommand(std::string text) : text_(text) {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        buffer.insert(cursorPosition, text_);
        cursorPosition += text_.size();
    }

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitInsertTextCommand(*this);
    }

private:
    std::string text_;
};

/* Удалить текст */
class DeleteTextCommand : public ICommand {
public:
    DeleteTextCommand () {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        if (editor.HasSelection()) {
            buffer.erase(editor.GetSelection().first, editor.GetSelection().second);
            editor.UnselectText();
        }
        else {
            buffer.erase(cursorPosition, cursorPosition+1);
        }
    } 

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitDeleteTextCommand(*this);
    }   

};

/* Скопировать текст */
class CopyTextCommand : public ICommand {
public:
    CopyTextCommand(std::size_t selectionSize) : selectionSize_(selectionSize) {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        if (!editor.HasSelection()) {
            clipboard = buffer;
        }
        else {
            clipboard = buffer.substr(cursorPosition, editor.GetSelection().second-editor.GetSelection().first);
            editor.UnselectText();
        }
    }

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitCopyTextCommand(*this);
    }
 
private:
    std::size_t selectionSize_;
};

/* Вставить скопированный текст */
class PasteTextCommand : public ICommand {
public:
    PasteTextCommand() {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        if (!editor.HasSelection()) {
            buffer.insert(cursorPosition, clipboard);
            cursorPosition += clipboard.size();
        }
        else {
            buffer.replace(editor.GetSelection().first, editor.GetSelection().second, clipboard);
            editor.UnselectText();
        }
    }   

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitPasteTextCommand(*this);
    }
};

/* Привести выделенный текст в ВЕРХНИЙ регистр */
class UppercaseTextCommand : public ICommand {
public:
    UppercaseTextCommand (size_t selectionSize) :selectionSize_(selectionSize) {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        for (size_t i = cursorPosition; i < editor.GetSelection().second; i++) {
            if (buffer[i] > 96 && buffer[i] < 123) {
                buffer[i] -= 32;
            }
        }
    }

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitUppercaseTextCommand(*this);
    }

private:
    size_t selectionSize_;
};

/* Привести выделенный текст в нижний регистр */
class LowercaseTextCommand : public ICommand {
public:
    LowercaseTextCommand(size_t selectionSize) : selectionSize_(selectionSize) {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        for (size_t i = cursorPosition; i < editor.GetSelection().second; i++) {
            if (buffer[i] > 64 && buffer[i] < 91) {
                buffer[i] += 32;
            }
        }
    }

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitLowercaseTextCommand(*this);
    }

private:
    size_t selectionSize_;
};

/* Перенести курсор в конец строки */
class MoveToEndCommand : public ICommand {
public:
    MoveToEndCommand() {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        cursorPosition = buffer.size();
        if (buffer[cursorPosition-1] == '\n')
            --cursorPosition;
    }

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitMoveToEndCommand(*this);
    }
};

/* Перенести курсор в начало строки */
class MoveToStartCommand : public ICommand {
public:
    MoveToStartCommand () {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        if (cursorPosition == 0)
            return;
        size_t copy = cursorPosition-1;
        while (copy > 0 && buffer[copy] != '\n') {
            copy--;
        }
        if (copy == 0)
            cursorPosition = 0;
        else
            cursorPosition = copy+1;
    } 

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitMoveToStartCommand(*this);
    }   
};

/* Удалить часть строки, начиная от позиции курсора до первого пробела или конца строки */
class DeleteWordCommand : public ICommand {
public:
    DeleteWordCommand() {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        if (cursorPosition == buffer.size())
            return;
        size_t copy = cursorPosition;
        while (copy < buffer.size()) {
            copy++;
            if (buffer[copy] == ' ')
                break;
        }
        buffer.erase(cursorPosition, copy-cursorPosition);
    }     

    void AcceptVisitor(CommandVisitor& visitor) override {
        visitor.VisitDeleteWordCommand(*this);
    }
};

/* Макрос */
class MacroCommand : public ICommand {
public: 
    MacroCommand (std::list<CommandPtr>& subcommands) :subcommands_(subcommands) {}

    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) override {
        auto c = subcommands_.begin();
        while (c != subcommands_.end()) {
            editor.ApplyCommand(*c);
            ++c;
        }
    }

    void AcceptVisitor(CommandVisitor& visitor) {
        auto c = subcommands_.begin();
        while (c != subcommands_.end()) {
            (*c)->AcceptVisitor(visitor);
            ++c;
        }
    }

private:
    std::list<CommandPtr> subcommands_;
};

//--------------------------------------------------------------

CommandBuilder::CommandBuilder() :selectionSize_(0), logStreamPtr_(nullptr) {}

CommandPtr CommandBuilder::build() {
    CommandPtr commandptr = nullptr;
    if (type_ == CommandBuilder::Type::MoveCursorLeft) {
        commandptr = std::make_shared<MoveCursorLeftCommand>();
    }
    else if (type_ == CommandBuilder::Type::MoveCursorRight) {
        commandptr = std::make_shared<MoveCursorRightCommand>();
    }
    else if (type_ == CommandBuilder::Type::MoveCursorUp) {
        commandptr = std::make_shared<MoveCursorUpCommand>();
    }
    else if (type_ == CommandBuilder::Type::MoveCursorDown) {
        commandptr = std::make_shared<MoveCursorDownCommand>();
    }
    else if (type_ == CommandBuilder::Type::PasteText) {
        commandptr = std::make_shared<PasteTextCommand>();
    }
    else if (type_ == CommandBuilder::Type::InsertText) {
        commandptr = std::make_shared<InsertTextCommand>(text_);
    }
    else if (type_ == CommandBuilder::Type::DeleteText) {
        commandptr = std::make_shared<DeleteTextCommand>();
    }
    else if (type_ == CommandBuilder::Type::SelectText) {
        commandptr = std::make_shared<SelectTextCommand>(selectionSize_);
    }
    else if (type_ == CommandBuilder::Type::CopyText) {
        commandptr = std::make_shared<CopyTextCommand>(selectionSize_);
    }
    else if (type_ == CommandBuilder::Type::UppercaseText) {
        commandptr = std::make_shared<UppercaseTextCommand>(selectionSize_);
    }
    else if (type_ == CommandBuilder::Type::LowercaseText) {
        commandptr = std::make_shared<LowercaseTextCommand>(selectionSize_);
    }
    else if (type_ == CommandBuilder::Type::MoveToEnd) {
        commandptr = std::make_shared<MoveToEndCommand>();
    }
    else if (type_ == CommandBuilder::Type::MoveToStart) {
        commandptr = std::make_shared<MoveToStartCommand>();
    }
    else if (type_ == CommandBuilder::Type::DeleteWord) {
        commandptr = std::make_shared<DeleteWordCommand>();
    }
    else if (type_ == CommandBuilder::Type::Macro) {
        commandptr = std::make_shared<MacroCommand>(subcommands_);
    }

    if (logStreamPtr_ != nullptr) {
        commandptr = std::make_shared<LoggedCommandWrapper>(*logStreamPtr_, commandptr);
    }
    return commandptr;
}

CommandBuilder& CommandBuilder::WithType(Type type) {
    type_ = type;
    return *this;
}

CommandBuilder& CommandBuilder::SelectionSize(size_t SelectionSize) {
    selectionSize_ = SelectionSize;
    return *this;
}

CommandBuilder& CommandBuilder::Text(std::string text) {
    text_ = text;
    return *this;
}

CommandBuilder& CommandBuilder::LogTo(std::ostream& logStream) {
    logStreamPtr_ = &logStream;
    return *this;
}

CommandBuilder& CommandBuilder::AddSubcommand(CommandPtr subcommand) {
    subcommands_.push_back(subcommand);
    return *this;
}

//-----------------------------------------------------------------

CommandLoggerVisitor::CommandLoggerVisitor(std::ostream& logstream) :logStream_(logstream) {}

void CommandLoggerVisitor::VisitMoveCursorLeftCommand(MoveCursorLeftCommand& command) {
    logStream_ << "h";
}

void CommandLoggerVisitor::VisitMoveCursorRightCommand(MoveCursorRightCommand& command) {
    logStream_ << "l";
}

void CommandLoggerVisitor::VisitMoveCursorUpCommand(MoveCursorUpCommand& command) {
    logStream_ << "k";
}

void CommandLoggerVisitor::VisitMoveCursorDownCommand(MoveCursorDownCommand& command) {
    logStream_ << "j";
}

void CommandLoggerVisitor::VisitSelectCommand(SelectTextCommand& command) {
    logStream_ << "v";
}

void CommandLoggerVisitor::VisitInsertTextCommand(InsertTextCommand& command) {
    logStream_ << "i";
}

void CommandLoggerVisitor::VisitDeleteTextCommand(DeleteTextCommand& command) {
    logStream_ << "d";
}

void CommandLoggerVisitor::VisitCopyTextCommand(CopyTextCommand& command) {
    logStream_ << "y";
}

void CommandLoggerVisitor::VisitPasteTextCommand(PasteTextCommand& command) {
    logStream_ << "p";
}

void CommandLoggerVisitor::VisitUppercaseTextCommand(UppercaseTextCommand& command) {
    logStream_ << "U";
}

void CommandLoggerVisitor::VisitLowercaseTextCommand(LowercaseTextCommand& command) {
    logStream_ << "u";
}

void CommandLoggerVisitor::VisitMoveToEndCommand(MoveToEndCommand& command) {
    logStream_ << "$";
}

void CommandLoggerVisitor::VisitMoveToStartCommand(MoveToStartCommand& command) {
    logStream_ << "0";
}

void CommandLoggerVisitor::VisitDeleteWordCommand(DeleteWordCommand& command) {
    logStream_ << "dE";
}