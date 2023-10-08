$(document).ready(function(){
  $('#aboutMe .profile-description-container').on({
    'mouseover': () => {
      $('.profile-name').css({
        'color': '#FB96E2',
      }).text('Ry2uko「リツコ」');
      $('#aboutMe .img-thumbnail').attr('src', './images/profile2.jpg');
    },
    'mouseout': () => {
      $('.profile-name').css({
        'color': '#8EE3F7',
      }).text('Gabriel Fradz');
      $('#aboutMe .img-thumbnail').attr('src', './images/profile.jpg');
    },
  });

  $('#projectsCarousel').on('slide.bs.carousel', e => {
    let projectName = $(e.relatedTarget).find('img').attr('data-project-name');
    $('#aboutMeProjects .project-title').text(projectName);
  })
});