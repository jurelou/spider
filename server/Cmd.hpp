#ifndef                                       CMD_HPP_
# define                                      CMD_HPP_

#include                                      <boost/enable_shared_from_this.hpp>
#include                                      "SocketIO.hpp"

class                                         Cmd : public boost::enable_shared_from_this<Cmd> {
  typedef                                     void (Cmd::*Actions)(int);

  public:
    Cmd();
    ~Cmd();
    static boost::shared_ptr<Cmd>             create();
    void                                      manageCmd(const std::vector<std::string>& cmd);
    void                                      commandPause(int id);
    void                                      commandStop(int id);
    void                                      commandResume(int id);
    void                                      commandScreenshot(int id);
    void                                      commandStatus(int id);
    void                                      list();
    void                                      updatePool(const std::vector<boost::shared_ptr<SocketIO> >& clientPool);
  private:
    std::vector<boost::shared_ptr<SocketIO> > clientPool;
    std::vector<std::tuple<std::string, Actions> >  actions;
};

#endif                                        /* !CMD_HPP_ */