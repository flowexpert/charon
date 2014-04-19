#ifndef STATEMENTINTF_H
#define STATEMENTINTF_H

class StatementIntf
{
public:
	virtual bool operator()() const =0;
	operator bool() const
	{
		return this->operator ()();
	}
    virtual void reset()
    {
        return;
    }
};

#endif // STATEMENTINTF_H
